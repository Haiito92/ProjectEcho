// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values for this component's properties
#include <RecordManager/RecordableComponent.h>

#include <RecordManager/RecordManagerSubsystem.h>

#include "EchoSystem.h"
#include "RecordManager/RecordKeysStructs.h"
#include "Tools/Debug/EchoDebug.h"

URecordableComponent::URecordableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URecordableComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get Root Component as Primitive Component
	if (bHandlePhysicsOfMesh)
	{
		PhysicsComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	}
}

void URecordableComponent::RecordKey(const float& CurrentTimeKey)
{
	FRecordTransformKey TransformKey;
	TransformKey.TimeKey = CurrentTimeKey;
	TransformKey.Position = GetOwner()->GetActorLocation();
	TransformKey.Rotation = GetOwner()->GetActorRotation();
	TransformKey.Scale = GetOwner()->GetActorScale();
	
	TransformKeys.Add(TransformKey);
	TransformKeys.Sort([](const FRecordTransformKey& A, const FRecordTransformKey& B)
	{
		return A.TimeKey < B.TimeKey;
	});
	
	if (bHandlePhysicsOfMesh && IsValid(PhysicsComponent))
	{
		FRecordPhysicsKey PhysicsKey;
		PhysicsKey.TimeKey = CurrentTimeKey;
		PhysicsKey.LinearVelocity = PhysicsComponent->GetPhysicsLinearVelocity();
		PhysicsKey.AngularVelocity = PhysicsComponent->GetPhysicsAngularVelocityInDegrees();
		
		PhysicsKeys.Add(PhysicsKey);
		PhysicsKeys.Sort([](const FRecordPhysicsKey& A, const FRecordPhysicsKey& B)
		{
			return A.TimeKey < B.TimeKey;
		});
	}
}

void URecordableComponent::ReplayKey(const float& PreviousTimeKey, const float& CurrentTimeKey)
{
	const FRecordTransformKey* NextTransformKey = FindNextTransformKey(CurrentTimeKey);
	const FRecordTransformKey* PreviousTransformKey = FindPreviousTransformKey(CurrentTimeKey);
	if (NextTransformKey != nullptr && PreviousTransformKey != nullptr)
	{
		//Place Actor according to previous and next TransformKey 
		float lerpValue = (CurrentTimeKey - PreviousTransformKey->TimeKey) / (NextTransformKey->TimeKey - PreviousTransformKey->TimeKey);
		GetOwner()->SetActorLocation(FMath::Lerp(PreviousTransformKey->Position, NextTransformKey->Position, lerpValue));
		GetOwner()->SetActorRotation(FMath::Lerp(PreviousTransformKey->Rotation, NextTransformKey->Rotation, lerpValue));
		GetOwner()->SetActorScale3D(FMath::Lerp(PreviousTransformKey->Scale, NextTransformKey->Scale, lerpValue));
	}
}

void URecordableComponent::ReplayFirstKey()
{
	if (TransformKeys.IsEmpty()) return;
	GetOwner()->SetActorLocation(TransformKeys[0].Position);
	GetOwner()->SetActorRotation(TransformKeys[0].Rotation);
	GetOwner()->SetActorScale3D(TransformKeys[0].Scale);
	if (bHandlePhysicsOfMesh)
	{
		if (PhysicsComponent->IsSimulatingPhysics())
		{
			PhysicsComponent->SetPhysicsLinearVelocity(FVector(0,0,0));
			PhysicsComponent->SetPhysicsAngularVelocityInDegrees(FVector(0,0,0));
		}
	}
	UEchoDebug::DrawSphere(GetWorld(), EEchoSystem::Record, TransformKeys[0].Position, 10, 12, FColor::Turquoise, 3, 1);
	
}

void URecordableComponent::MarkAsCurrentlyInteracted()
{
	bIsInteractedWith = true;
}

void URecordableComponent::UnmarkAsCurrentlyInteracted()
{
	bIsInteractedWith = false;
}

void URecordableComponent::StartRewind()
{
	if (bHandlePhysicsOfMesh && IsValid(PhysicsComponent))
	{
		PhysicsComponent->SetSimulatePhysics(false);
		PhysicsComponent->SetPhysicsLinearVelocity(FVector(0,0,0));
		PhysicsComponent->SetPhysicsAngularVelocityInDegrees(FVector(0,0,0));
	}
	OnStartRewind.Broadcast();
}

void URecordableComponent::StopRewind(const float& CurrentTimeKey, bool bForceReset)
{
	if (bHandlePhysicsOfMesh && IsValid(PhysicsComponent))
	{
		if (CurrentTimeKey > InteractionKeys[0].TimeKey)
		{
			if (!bIsInteractedWith || bForceReset) PhysicsComponent->SetSimulatePhysics(true);
			const FRecordPhysicsKey* NextPhysicsKey = FindNextPhysicsKey(CurrentTimeKey);
			const FRecordPhysicsKey* PreviousPhysicsKey = FindPreviousPhysicsKey(CurrentTimeKey);
			if (NextPhysicsKey != nullptr && PreviousPhysicsKey != nullptr)
			{
				//Place Actor according to previous and next PhysicsKey 
				float lerpValue = (CurrentTimeKey - PreviousPhysicsKey->TimeKey) / (NextPhysicsKey->TimeKey - PreviousPhysicsKey->TimeKey);
				FVector Velocity = FMath::Lerp(PreviousPhysicsKey->LinearVelocity, NextPhysicsKey->LinearVelocity, lerpValue);
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Replaying Velocity : " + Velocity.ToString());
				PhysicsComponent->SetPhysicsLinearVelocity(Velocity);
				PhysicsComponent->SetPhysicsAngularVelocityInDegrees(FMath::Lerp(PreviousPhysicsKey->AngularVelocity, NextPhysicsKey->AngularVelocity, lerpValue));
			}
		}
		else
		{
			if (!bIsInteractedWith || bForceReset) PhysicsComponent->SetSimulatePhysics(true);
			PhysicsComponent->SetPhysicsLinearVelocity(FVector(0,0,0));
			PhysicsComponent->SetPhysicsAngularVelocityInDegrees(FVector(0,0,0));
		}
	}
	if (CurrentTimeKey > InteractionKeys[0].TimeKey)
	{
		ClearKeysPastCurrentKey(CurrentTimeKey);
	}
	OnStopRewind.Broadcast();
}

void URecordableComponent::StartRecording(const FRecordInteractionKey& FirstInteractionKey)
{
	if (!IsRecording())
	{
		bIsRecording = true;
		RegisterInteractionKey(FirstInteractionKey);
	}
}

void URecordableComponent::RegisterInteractionKey(const FRecordInteractionKey& InteractionKey)
{ 
	InteractionKeys.Add(InteractionKey);
	InteractionKeys.Sort([](const FRecordInteractionKey& A, const FRecordInteractionKey& B)
	{
		return A.TimeKey < B.TimeKey;
	});
}

void URecordableComponent::HandleTimelineDestruction(const int& RecordTimelineIndex)
{
	InteractionKeys.RemoveAll([&](const FRecordInteractionKey& InteractionKey)
	{
		return InteractionKey.RecordTimelineIndex == RecordTimelineIndex;
	});
	if (InteractionKeys.IsEmpty())
	{
		StopRecording(true);
	}
	else
	{
		ClearKeysBeforeCurrentKey(InteractionKeys[0].TimeKey);
	}
}

void URecordableComponent::StopRecording(bool bForceStopRecording)
{
	if (!bIsInteractedWith || bForceStopRecording)
	{
		bIsRecording = false;
		InteractionKeys.Empty();
	}
	if (InteractionKeys.Num() > 1)
	{
		for (int i = 1; i < InteractionKeys.Num() - 2; i++)
		{
			InteractionKeys.RemoveAt(i);
		}
	}
	TransformKeys.Empty();
	PhysicsKeys.Empty();
}

bool URecordableComponent::IsRecording() const
{
	return bIsRecording;
}

bool URecordableComponent::IsCurrentlyInteractedWith() const
{
	return bIsInteractedWith;
}

float URecordableComponent::GetFirstInteractedKey() const
{
	if (InteractionKeys.IsEmpty()) return -1;
	return InteractionKeys[0].TimeKey;
}

void URecordableComponent::ClearKeysPastCurrentKey(const float& CurrentTimeKey)
{
	if (!TransformKeys.IsEmpty())
	{
		TransformKeys.RemoveAll([&](const FRecordTransformKey& TransformKey)
		{
			return TransformKey.TimeKey > CurrentTimeKey;
		});
	}
	if (!PhysicsKeys.IsEmpty())
	{
		PhysicsKeys.RemoveAll([&](const FRecordPhysicsKey& PhysicsKey)
		{
			return PhysicsKey.TimeKey > CurrentTimeKey;
		});
	}
	if (!InteractionKeys.IsEmpty())
	{
		InteractionKeys.RemoveAll([&](const FRecordInteractionKey& InteractionKey)
		{
			return InteractionKey.TimeKey > CurrentTimeKey;
		});
	}
}

void URecordableComponent::ClearKeysBeforeCurrentKey(const float& CurrentTimeKey)
{
	if (!TransformKeys.IsEmpty())
	{
		TransformKeys.RemoveAll([&](const FRecordTransformKey& TransformKey)
		{
			return TransformKey.TimeKey < CurrentTimeKey;
		});
	}
	if (!PhysicsKeys.IsEmpty())
	{
		PhysicsKeys.RemoveAll([&](const FRecordPhysicsKey& PhysicsKey)
		{
			return PhysicsKey.TimeKey < CurrentTimeKey;
		});
	}
	if (!InteractionKeys.IsEmpty())
	{
		InteractionKeys.RemoveAll([&](const FRecordInteractionKey& InteractionKey)
		{
			return InteractionKey.TimeKey < CurrentTimeKey;
		});
	}
}

const FRecordTransformKey* URecordableComponent::FindPreviousTransformKey(const float& CurrentTimeKey)
{
	if (TransformKeys.IsEmpty()) return nullptr;
	const FRecordTransformKey* key = nullptr;
	for (const FRecordTransformKey& TransformKey : TransformKeys)
	{
		if (TransformKey.TimeKey <= CurrentTimeKey)
		{
			key = &TransformKey;
		}
		else
		{
			break;
		}
	}
	return key;
}

const FRecordTransformKey* URecordableComponent::FindNextTransformKey(const float& CurrentTimeKey)
{
	if (TransformKeys.IsEmpty()) return nullptr;
	for (const FRecordTransformKey& TransformKey : TransformKeys)
	{
		if (TransformKey.TimeKey > CurrentTimeKey)
		{
			return &TransformKey;
		}
	}
	return nullptr;
}

const FRecordPhysicsKey* URecordableComponent::FindPreviousPhysicsKey(const float& CurrentTimeKey)
{
	if (PhysicsKeys.IsEmpty()) return nullptr;
	const FRecordPhysicsKey* key = nullptr;
	for (const FRecordPhysicsKey& PhysicsKey : PhysicsKeys)
	{
		if (PhysicsKey.TimeKey <= CurrentTimeKey)
		{
			key = &PhysicsKey;
		}
		else
		{
			break;
		}
	}
	return key;
}

const FRecordPhysicsKey* URecordableComponent::FindNextPhysicsKey(const float& CurrentTimeKey)
{
	if (PhysicsKeys.IsEmpty()) return nullptr;
	for (const FRecordPhysicsKey& PhysicsKey : PhysicsKeys)
	{
		if (PhysicsKey.TimeKey > CurrentTimeKey)
		{
			return &PhysicsKey;
		}
	}
	return nullptr;
}

