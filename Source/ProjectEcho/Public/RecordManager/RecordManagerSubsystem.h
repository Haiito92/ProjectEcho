// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RecordManagerSubsystem.generated.h"

/**
 * 
 */

//Key used to save the position of an element at a set timekey 
USTRUCT(Blueprintable)
struct FRecordTransformKey
{
	GENERATED_BODY()
	
	float TimeKey;
	FVector Position;
	FRotator Rotation;
	FVector Scale;
};

#pragma region Timeline Struct
USTRUCT(Blueprintable)
struct FTimeline
{
	GENERATED_BODY()
	
	//Actor used to show Replay of Timeline (Echo)
	TObjectPtr<AActor> EchoActor = nullptr;
	
	//List of TransformKeys
	TArray<FRecordTransformKey> TransformKeys;
	
	//TimeKey of Start of Timeline (from Global Timeline)
	float StartTimeKey;

	//Get Closest Next Key
	UFUNCTION(BlueprintCallable)
	const FRecordTransformKey* GetNextTransformKey(const float* TimeKey) const;
	
	//Get Closest Previous Key
	UFUNCTION(BlueprintCallable)
	const FRecordTransformKey* GetPreviousTransformKey(const float* TimeKey) const;
	
	//Get Last Key of Timeline (End of Timeline, in Local Time)
	const float* GetLastTimeKey();
	
	//Save Echo Actor for Replays
	UFUNCTION(BlueprintCallable)
	void RegisterEchoActor(AActor* InEchoActor);
	
	//Record current Transform into TransformKey in Timeline List
	UFUNCTION(BlueprintCallable)
	void RecordTransformKey(AActor* RecordedActor, const float* CurrentTimeKey);
	
	/* Replay Function
	 * Play Current Frame of the Replay with given PreviousKey Played and CurrentTimeKey 
	 * This function will : 
	 * - Calculate the Transform of the Actor based on last and next TransformKey
	 */
	UFUNCTION(BlueprintCallable)
	void PlayReplay(const float* PreviousKey,const float* CurrentTimeKey, bool bIsInRewind) -> void;
};

inline const FRecordTransformKey* FTimeline::GetNextTransformKey(const float* TimeKey) const
{
	for (const FRecordTransformKey& TransformKey : TransformKeys)
	{
		if (TransformKey.TimeKey >= *TimeKey)
		{
			return &TransformKey;
		}
	}
	return nullptr;
}

inline const FRecordTransformKey* FTimeline::GetPreviousTransformKey(const float* TimeKey) const
{
	const FRecordTransformKey* key = nullptr;
	for (const FRecordTransformKey& TransformKey : TransformKeys)
	{
		if (TransformKey.TimeKey <= *TimeKey)
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

inline const float* FTimeline::GetLastTimeKey()
{
	return &TransformKeys[TransformKeys.Num() - 1].TimeKey;
}

inline void FTimeline::RegisterEchoActor(AActor* InEchoActor)
{
	EchoActor = InEchoActor;
}

inline void FTimeline::RecordTransformKey(AActor* RecordedActor, const float* CurrentTimeKey)
{
	//Create and Init values of TransformKey
	FRecordTransformKey TransformKey;
	TransformKey.TimeKey = *CurrentTimeKey;
	TransformKey.Position = RecordedActor->GetActorLocation();
	TransformKey.Rotation = RecordedActor->GetActorRotation();
	TransformKey.Scale = RecordedActor->GetActorScale();
	
	//Add it to the list and Sort the list (list has to be in order)
	TransformKeys.Add(TransformKey);
	TransformKeys.Sort([](const FRecordTransformKey& A, const FRecordTransformKey& B)
	{
		return A.TimeKey < B.TimeKey;
	});
}

inline void FTimeline::PlayReplay(const float* PreviousKey,const float* CurrentTimeKey, bool bIsInRewind)
{
	if (GetLastTimeKey() < CurrentTimeKey) return;
	
	//Play Transform Key (Lerp between two closest Keys)
	const FRecordTransformKey* PreviousTransformKey = GetPreviousTransformKey(CurrentTimeKey);
	const FRecordTransformKey* NextTransformKey = GetNextTransformKey(CurrentTimeKey);
	
	//Place Actor according to previous and next TransformKey 
	float lerpValue = (*CurrentTimeKey - PreviousTransformKey->TimeKey) / (NextTransformKey->TimeKey - PreviousTransformKey->TimeKey);
	EchoActor->SetActorLocation(FMath::Lerp(PreviousTransformKey->Position, NextTransformKey->Position, lerpValue));
	EchoActor->SetActorRotation(FMath::Lerp(PreviousTransformKey->Rotation, NextTransformKey->Rotation, lerpValue));
	EchoActor->SetActorScale3D(FMath::Lerp(PreviousTransformKey->Scale, NextTransformKey->Scale, lerpValue));
}

#pragma endregion

UCLASS()
class PROJECTECHO_API URecordManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
	virtual void Tick(float DeltaTime) override;
};

