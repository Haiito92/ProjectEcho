// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values for this component's properties
#include <RecordManager/RecordableComponent.h>

#include <RecordManager/RecordManagerSubsystem.h>

#include "RecordManager/RecordKeysStructs.h"

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

void URecordableComponent::StopRewind(const float& CurrentTimeKey)
{
	if (bHandlePhysicsOfMesh && IsValid(PhysicsComponent))
	{
		PhysicsComponent->SetSimulatePhysics(true);
		if (CurrentTimeKey > FirstInteractedKey)
		{
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
			PhysicsComponent->SetPhysicsLinearVelocity(FVector(0,0,0));
			PhysicsComponent->SetPhysicsAngularVelocityInDegrees(FVector(0,0,0));
		}
	}
	if (CurrentTimeKey > FirstInteractedKey)
	{
		ClearKeysPastCurrentKey(CurrentTimeKey);
	}
	OnStopRewind.Broadcast();
}

void URecordableComponent::StartRecording(const float& CurrentTimeKey)
{
	if (!IsRecording())
	{
		bIsRecording = true;
		FirstInteractedKey = CurrentTimeKey;
	}
}

void URecordableComponent::StopRecording()
{
	bIsRecording = false;
	FirstInteractedKey = -1;
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

const float& URecordableComponent::GetFirstInteractedKey() const
{
	return FirstInteractedKey;
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

