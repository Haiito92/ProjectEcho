// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Subsystems/WorldSubsystem.h"
#include "UObject/ObjectPtr.h"

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
	
	//Whether Timeline is currently active and showing a Replay (In Rewind or not)
	bool bIsActive = false;

	//Get Closest Next Key
	UFUNCTION(BlueprintCallable)
	const FRecordTransformKey* GetNextTransformKey(const float& TimeKey) const;
	
	//Get Closest Previous Key
	UFUNCTION(BlueprintCallable)
	const FRecordTransformKey* GetPreviousTransformKey(const float& TimeKey) const;
	
	//Get Last Key of Timeline (End of Timeline, in Local Time)
	const float& GetLastTimeKey();
	
	//Save Echo Actor for Replays
	UFUNCTION(BlueprintCallable)
	void RegisterEchoActor(AActor* InEchoActor);
	
	//Record current Transform into TransformKey in Timeline List
	UFUNCTION(BlueprintCallable)
	void RecordTransformKey(AActor* RecordedActor, const float& CurrentTimeKey);
	
	/* Replay Function
	 * Play Current Frame of the Replay with given PreviousKey Played and CurrentTimeKey 
	 * This function will : 
	 * - Calculate the Transform of the Actor based on last and next TransformKey
	 */
	UFUNCTION(BlueprintCallable)
	void PlayReplay(const float& PreviousKey,const float& CurrentTimeKey, bool bIsInRewind) -> void;
	
	UFUNCTION(BlueprintCallable)
	void ActivateTimeline(bool bInIsActive);
};

USTRUCT(Blueprintable)
struct FGlobalTimeline
{
	GENERATED_BODY()
	
	TArray<FTimeline> Timelines;
	
	int MaxSlots = 5;
	
	//Play CurrentFrame for all Active Timelines, activate timelines that have not yet been activated
	void Play(const float& PreviousTimeKey, const float& CurrentTimeKey, bool bIsInRewind, bool& bOutHasReachedEnd);
	
	//Whether GlobalTimeline has an available slot to start recording in it
	bool HasAvailableTimelineSlot();
};

#pragma endregion

UCLASS()
class PROJECTECHO_API URecordManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
	virtual void Tick(float DeltaTime) override;
protected:
	FGlobalTimeline GlobalTimeline;
	
private:
};

