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

	// ...
	
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
}

void URecordableComponent::ReplayKey(const float& PreviousTimeKey, const float& CurrentTimeKey)
{
	const FRecordTransformKey* NextTransformKey = FindClosestTransformKey(CurrentTimeKey, true);
	const FRecordTransformKey* PreviousTransformKey = FindClosestTransformKey(CurrentTimeKey, false);
	if (NextTransformKey != nullptr || PreviousTransformKey != nullptr)
	{
		//Place Actor according to previous and next TransformKey 
		float lerpValue = (CurrentTimeKey - PreviousTransformKey->TimeKey) / (NextTransformKey->TimeKey - PreviousTransformKey->TimeKey);
		GetOwner()->SetActorLocation(FMath::Lerp(PreviousTransformKey->Position, NextTransformKey->Position, lerpValue));
		GetOwner()->SetActorRotation(FMath::Lerp(PreviousTransformKey->Rotation, NextTransformKey->Rotation, lerpValue));
		GetOwner()->SetActorScale3D(FMath::Lerp(PreviousTransformKey->Scale, NextTransformKey->Scale, lerpValue));
	}
}

void URecordableComponent::StartRewind()
{
}

const FRecordTransformKey* URecordableComponent::FindClosestTransformKey(const float& CurrentTimeKey, bool bFindNextOne)
{
	if (TransformKeys.IsEmpty()) return nullptr;
	const FRecordTransformKey* key = nullptr;
	for (const FRecordTransformKey& TransformKey : TransformKeys)
	{
		if (bFindNextOne)
		{
			if (TransformKey.TimeKey >= CurrentTimeKey)
			{
				return &TransformKey;
			}
		}
		else if (TransformKey.TimeKey <= CurrentTimeKey)
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

