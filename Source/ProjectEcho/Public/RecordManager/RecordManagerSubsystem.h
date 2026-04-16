// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecordKeysStructs.h"
#include "RecordManagerSettings.h"
#include "GameFramework/Actor.h"
#include "Subsystems/WorldSubsystem.h"
#include "UObject/ObjectPtr.h"

#include "RecordManagerSubsystem.generated.h"

/**
 * 
 */

class URecordableComponent;
enum class ERecordedAction : uint8;

class URecordManagerSettings;

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
	 * - Execute Actions that occured between previousTimeKey and currentTimeKey
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
	
	//Find First Available Timeline Slot
	int GetFirstAvailableTimelineSlot() const;
	
	//Find Last Time Key of the Global Timeline (Last Key of Last Timeline played)
	float GetLastTimeKey() const;
	
	//Get Length of GlobalTimeline
	float GetLength() const;
	
	//Add Timeline to Global Timeline
	void RegisterTimeline(const int& TimelineIndex, const FEchoTimeline& Timeline, TObjectPtr<URecordManagerSettings> Settings);
	
	//Destroy Current Timeline
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
	
	UFUNCTION(BlueprintCallable)
    void StartRecord(AActor* InRecordedActor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanStartRecord() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanStopRecord() const;
    
    UFUNCTION(BlueprintCallable)
    void StopRecord();
	
	UFUNCTION()
	void StartPlayerRewind();
	
	UFUNCTION()
	void StopPlayerRewind();
	
	UFUNCTION()
	void StartRewind();
	
	UFUNCTION()
	void StopRewind();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsRecording();
	
	//Destroys the Timeline at the Slot currently selected (if there is one) 
    UFUNCTION(BlueprintCallable)
    void DestroySelectedTimeline();
    	
    // Increment the Selected Slot Value, if reaches end, goes back to first Slot
    UFUNCTION(BlueprintCallable)
    void IncrementSelectedSlot();
    	
    // Decrement the Selected Slot Value, if reaches beginning, goes back to last Slot
    UFUNCTION(BlueprintCallable)
    void DecrementSelectedSlot();
	
	UFUNCTION()
	void OnRecordableInteractedWith(URecordableComponent* Self, bool bShouldRecord);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStartRecording, float, CurrentTimeKey, int, TimelineIndex);
	UPROPERTY(BlueprintAssignable)
	FOnStartRecording OnStartRecording;	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopRecording);
	UPROPERTY(BlueprintAssignable)
	FOnStopRecording OnStopRecording;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartPlayerRewinding);
	FOnStartPlayerRewinding OnStartPlayerRewinding;	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopPlayerRewinding);
	FOnStopPlayerRewinding OnStopPlayerRewinding;
	
private:
	virtual void Tick(float DeltaTime) override;
	
	//Handle Replay of Player Rewind (Placement of Actions
	void PlayPlayerRewind(const float& TimeKey);
	
protected:
	FGlobalTimeline GlobalTimeline;

	//Current Recording Timeline;
	FEchoTimeline RecordingTimeline;
	
	UPROPERTY()
	int CurrentRecordingTimelineIndex = -1;
	
	UPROPERTY()
	TObjectPtr<AActor> RecordedActor = nullptr;
	
	//Current TimeKey, used for Recording and Replays;
	float CurrentTimeKey = 0.0f;
	
	//Is Recording
	bool bIsRecording = false;
	
	//Is Timeline Replay in Rewind
	bool bIsInRewind = false;
	
	//Is Currently rewinding Player's Actions after a record
	bool bIsPlayerRewinding = false;
	
	//Current Selected Timeline Slot
	int SelectedSlot = 0;
	
	//Speed of Rewind (Calculated when rewind is Called
	float RewindSpeed = 0.f;
	
private:
	UPROPERTY()
	TObjectPtr<URecordManagerSettings> RecordManagerSettings = nullptr;
	
	UPROPERTY()
	TArray<TObjectPtr<URecordableComponent>> RecordableComponents;
	
	UPROPERTY()
	//Pool of EchoActor to display Timelines (avoid runtime Spawning)
	TArray<TObjectPtr<AEchoActor>> EchoActorsPool;
};

