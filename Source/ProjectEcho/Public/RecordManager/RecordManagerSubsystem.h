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
struct FEchoTimeline
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
	const FRecordTransformKey* GetNextTransformKey(const float& TimeKey) const;
	
	//Get Closest Previous Key
	const FRecordTransformKey* GetPreviousTransformKey(const float& TimeKey) const;
	
	//Get Last Key of Timeline (End of Timeline, in Local Time)
	const float& GetLastTimeKey() const;
	
	//Save Echo Actor for Replays
	void RegisterEchoActor(AActor* InEchoActor);
	
	//Record current Transform into TransformKey in Timeline List
	void RecordTransformKey(AActor* RecordedActor, const float& CurrentTimeKey);
	
	/* Replay Function
	 * Play Current Frame of the Replay with given PreviousKey Played and CurrentTimeKey 
	 * This function will : 
	 * - Calculate the Transform of the Actor based on last and next TransformKey
	 */
	void PlayReplay(const float& PreviousKey,const float& CurrentTimeKey, bool bIsInRewind);
	
	void ActivateTimeline(bool bInIsActive);
};
#pragma endregion

#pragma region GlobalTimelineStruct

USTRUCT(Blueprintable)
struct FGlobalTimeline
{
	GENERATED_BODY()
	
	TArray<FEchoTimeline> Timelines;
	
	int MaxSlots = 5;
	
	//Play CurrentFrame for all Active Timelines, activate timelines that have not yet been activated
	void Play(const float& PreviousTimeKey, const float& CurrentTimeKey, bool bIsInRewind, bool& bOutHasReachedEnd);
	
	//Whether GlobalTimeline has an available slot to start recording in it
	bool HasAvailableTimelineSlot() const;
	
	float GetLastTimeKey() const;
	
	//Add Timeline to Global Timeline
	void RegisterTimeline(const FEchoTimeline& Timeline);
};

#pragma endregion

UCLASS()
class PROJECTECHO_API URecordManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
	virtual TStatId GetStatId() const override;
	
	UFUNCTION(BlueprintCallable)
	void StartRecord(AActor* InRecordedActor);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartRecording, float, CurrentTimeKey);
	FOnStartRecording OnStartRecording;
	
	UFUNCTION(BlueprintCallable)
	void StopRecord();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopRecording);
	FOnStopRecording OnStopRecording;
	
	virtual void Tick(float DeltaTime) override;
	
	//For Testing Purposes
	UFUNCTION(BlueprintCallable)
	void AssociateEchoToRecordingTimeline(AActor* Echo);
protected:
	FGlobalTimeline GlobalTimeline;

	//Current Recording Timeline;
	FEchoTimeline RecordingTimeline;
	
	TObjectPtr<AActor> RecordedActor = nullptr;
	
	//Current TimeKey, used for Recording and Replays;
	float CurrentTimeKey = 0.0f;
	
	//Is Recording
	bool bIsRecording = false;
	
private:
};

