// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecordManagerSettings.h"
#include "GameFramework/Actor.h"
#include "Subsystems/WorldSubsystem.h"
#include "UObject/ObjectPtr.h"

#include "RecordManagerSubsystem.generated.h"

/**
 * 
 */

enum class ERecordedAction : uint8;

class URecordManagerSettings;
//Key used to save the position of an element at a set timekey 
USTRUCT(Blueprintable)
struct FRecordTransformKey
{
	GENERATED_BODY()
	
	float TimeKey;
	FVector Position;
	FRotator Rotation;
	FVector Scale;
	FRotator ControlRotation;
};

USTRUCT()
struct FRecordActionKey
{
	GENERATED_BODY()
	
	float TimeKey;
	ERecordedAction Action;
};


#pragma region Timeline Struct
USTRUCT(Blueprintable)
struct FEchoTimeline
{
	GENERATED_BODY()
	
	//Actor used to show Replay of Timeline (Echo)
	TObjectPtr<AEchoActor> EchoActor = nullptr;
	
	//List of TransformKeys
	TArray<FRecordTransformKey> TransformKeys;
	//List of TransformKeys
	TArray<FRecordActionKey> ActionKeys;
	
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
	
	//Get All Action Keys between two Keys in given Array, returns true if has found ActionKeys
	bool GetActionKeys(const float& PreviousKey,const float& CurrentTimeKey, TArray<const FRecordActionKey*>& OutActionKeys) const;
	
	//Save Echo Actor for Replays
	void RegisterEchoActor(AEchoActor* InEchoActor);
	
	//Record current Transform into aTransformKey in Timeline List
	void RecordTransformKey(AActor* RecordedActor, const float& CurrentTimeKey);
	
	//Record All Actions executed between last record and now into an ActionKey in Timeline List
	void RecordActionKey(AActor* RecordedActor, const float& CurrentTimeKey);
	
	/* Replay Function
	 * Play Current Frame of the Replay with given PreviousKey Played and CurrentTimeKey 
	 * This function will : 
	 * - Calculate the Transform of the Actor based on last and next TransformKey
	 */
	void PlayReplay(const float& PreviousKey,const float& CurrentTimeKey, bool bIsInRewind);
	
	//Turns
	void ActivateTimeline(bool bInIsActive);
	
	//Called when Timeline is being Destroyed
	void OnDestroy();
};
#pragma endregion

#pragma region GlobalTimelineStruct

USTRUCT(Blueprintable)
struct FGlobalTimeline
{
	GENERATED_BODY()
	
	TMap<int, FEchoTimeline> Timelines;
	
	void Initiate(int InNbSlots);
	
	int NbSlots = 5;
	
	//Play CurrentFrame for all Active Timelines, activate timelines that have not yet been activated
	void Play(const float& PreviousTimeKey, const float& CurrentTimeKey, bool bIsInRewind, bool& bOutHasReachedEnd);
	
	//Whether GlobalTimeline has an available slot to start recording in it
	bool HasAvailableTimelineSlot() const;
	
	//Find Last Time Key of the Global Timeline (Last Key of Last Timeline played)
	float GetLastTimeKey() const;
	
	//Add Timeline to Global Timeline
	void RegisterTimeline(const FEchoTimeline& Timeline);
	
	void DestroyTimeline(int TimelineIndex, TArray<TObjectPtr<AEchoActor>>& OutEchoActorPool);
};

#pragma endregion

UCLASS()
class PROJECTECHO_API URecordManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
	virtual TStatId GetStatId() const override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void InitRecordManager(const int& NbTimelineSlot);

private:
	UFUNCTION(BlueprintCallable)
	void StartRecord(AActor* InRecordedActor);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartRecording, float, CurrentTimeKey);
	FOnStartRecording OnStartRecording;
	
	UFUNCTION(BlueprintCallable)
	void StopRecord();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopRecording);
	FOnStopRecording OnStopRecording;
	
	virtual void Tick(float DeltaTime) override;
	
	//Destroys the Timeline at the Slot currently selected (if there is one) 
	UFUNCTION(BlueprintCallable)
	void DestroySelectedTimeline();
	
	// Increment the Selected Slot Value, if reaches end, goes back to first Slot
	UFUNCTION(BlueprintCallable)
	void IncrementSelectedSlot();
	
	// Decrement the Selected Slot Value, if reaches beginning, goes back to last Slot
	UFUNCTION(BlueprintCallable)
	void DecrementSelectedSlot();
protected:
	FGlobalTimeline GlobalTimeline;

	//Current Recording Timeline;
	FEchoTimeline RecordingTimeline;
	
	UPROPERTY()
	TObjectPtr<AActor> RecordedActor = nullptr;
	
	//Current TimeKey, used for Recording and Replays;
	float CurrentTimeKey = 0.0f;
	
	//Is Recording
	bool bIsRecording = false;
	
	int SelectedSlot = 0;
	
private:
	UPROPERTY()
	TObjectPtr<URecordManagerSettings> RecordManagerSettings = nullptr;
	
	UPROPERTY()
	//Pool of EchoActor to display Timelines (avoid runtime Spawning)
	TArray<TObjectPtr<AEchoActor>> EchoActorsPool;
};

