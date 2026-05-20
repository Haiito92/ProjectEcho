// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecordKeysStructs.h"
#include "RecordManagerSettings.h"
#include "UIRecordStructs.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AEchoActor> EchoActor = nullptr;
	
	//TimeKey of Start of Timeline (from Global Timeline)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRecordTransformKey> TransformKeys;
	
	//List of Action Keys
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRecordActionKey> ActionKeys;
	
	//List of Rewind ActionKeys
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRecordActionKey> RewindActionKeys;
	
	//TimeKey of Start of Timeline (from Global Timeline)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTimeKey;
	
	//Whether Timeline is currently active and showing a Replay (In Rewind or not)
	UPROPERTY()
	bool bIsActive = false;

	//Get Closest Next Key
	const FRecordTransformKey* GetNextTransformKey(const float& TimeKey) const;
	
	//Get Closest Previous Key
	const FRecordTransformKey* GetPreviousTransformKey(const float& TimeKey) const;
	
	//Get Last Key of Timeline (End of Timeline, in Local Time)
	const float& GetLastTimeKey() const;
	
	//Get All Action Keys between two Keys in given Array, returns true if has found ActionKeys
	bool GetActionKeys(const float& PreviousKey,const float& CurrentTimeKey, bool bIsInRewind, TArray<FRecordActionKey>& OutActionKeys) const;
	
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
	
	//Play First Key of Timeline and Play Given Actions (Used to restore a State)
	void PlayFirstKey(TArray<FRecordedAction> RestoreFirstStateAction);
	
	//Turns
	void ActivateTimeline(bool bInIsActive);
	
	//Called by global timeline when rewind started
	void HandleRewindStarted(const float& CurrentTimeKey, bool bIsPlayerRewind);
	
	//Called by global timeline when rewind is finished
	void HandleRewindStopped(const float& CurrentTimeKey, bool bIsPlayerRewind);
		
	//Called by global timeline when Record is started
	void HandleRecordStarted(const float& CurrentTimeKey);
	
	//Called when Timeline is being Destroyed
	void OnDestroy();
	
	//Record First Action Key (Used when Starting to Record)
	void RecordFirstActionKeys(const TArray<FRecordedAction>& FirstActions);
};
#pragma endregion

#pragma region GlobalTimelineStruct

USTRUCT(Blueprintable)
struct FGlobalTimeline
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<int, FEchoTimeline> Timelines;
	
	void Initiate(int InNbSlots);

	UPROPERTY()
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
	
	// Called by Record Manager when rewind started
	void HandleRewindStarted(const float& CurrentTimeKey, bool bIsPlayerRewind = false);
	
	// Called by Record Manager when rewind stopped
	void HandleRewindStopped(const float& CurrentTimeKey, bool bIsPlayerRewind = false);
	
	// Called by Record Manager when record started
	void HandleRecordStarted(const float& CurrentTimeKey);
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
	/* Start Record
	 * RestoreStateAction -> Action to execute on First replay Only
	 * FirstActions -> Action that will be registered as First Action Key
	 */
    void StartRecord(AActor* InRecordedActor, const TArray<FRecordedAction>& RestoreStateAction, const TArray<FRecordedAction>& FirstActions);
	
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
	void StartRewind(bool bIsPlayerRewind = false);
	
	UFUNCTION()
	void StopRewind(bool bIsPlayerRewind = false);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsRecording();
	
	//Destroys the Timeline at the Slot currently selected (if there is one) 
    UFUNCTION(BlueprintCallable)
    void DestroySelectedTimeline();
    	
	//Destroys the Timeline at the index 
	UFUNCTION(BlueprintCallable)
	void DestroyTimeline(int TimelineIndex);
	
    // Increment the Selected Slot Value, if reaches end, goes back to first Slot
    UFUNCTION(BlueprintCallable)
    void IncrementSelectedSlot();
    	
    // Decrement the Selected Slot Value, if reaches beginning, goes back to last Slot
    UFUNCTION(BlueprintCallable)
    void DecrementSelectedSlot();
	
	//Select Precise Selected Slot Value, if Slot doesn't exit, value won't change
	UFUNCTION(BlueprintCallable)
	void SelectSlot(int Index, bool bCanSelectNonExistentTimeline = false);
	
	UFUNCTION()
	void OnRecordableInteractedWith(URecordableComponent* Self, bool bShouldRecord, int RecordTimelineIndex);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStartRecording, float, CurrentTimeKey, int, TimelineIndex, const FEchoColorStruct&, EchoColorInformations);
	UPROPERTY(BlueprintAssignable)
	FOnStartRecording OnStartRecording;	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopRecording);
	UPROPERTY(BlueprintAssignable)
	FOnStopRecording OnStopRecording;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartPlayerRewinding);
	UPROPERTY(BlueprintAssignable)
	FOnStartPlayerRewinding OnStartPlayerRewinding;	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopPlayerRewinding);
	UPROPERTY(BlueprintAssignable)
	FOnStopPlayerRewinding OnStopPlayerRewinding;
	
	//--- UI Events ---
	
	//Called When New Timeline Created (gives Timeline Index reference)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimelineCreated, int, CreatedTimelineIndex);
	UPROPERTY(BlueprintAssignable);
	FOnTimelineCreated OnTimelineCreated;
	
	//Called When New Timeline Destroyed (gives Timeline Index reference)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimelineDestroyed, int, DestroyedTimelineIndex);
	UPROPERTY(BlueprintAssignable);
	FOnTimelineDestroyed OnTimelineDestroyed;
	
	//Called When Timeline is Modified, Moved around (gives Timeline Index reference)
	// --- Not Yet Implemented ---
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimelineModified, int, ModifiedTimelineIndex);
	UPROPERTY(BlueprintAssignable)
	FOnTimelineModified OnTimelineModified;
	
	//Called On Tick after Timeline Replay Update (gives previous TimeKey and New (Current) TimeKey)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTimelineReplayUpdate, int, PreviousTimeKey, int, CurrentTimeKey, bool, bIsRecording);
	UPROPERTY(BlueprintAssignable)
	FOnTimelineReplayUpdate OnTimelineReplayUpdate;
	
	//Called On Timeline Selected Index Changed (gives Selected Timeline Index)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimelineSelected, int, PreviouslySelectedTimelineIndex, int, SelectedTimelineIndex);
	UPROPERTY(BlueprintAssignable)
	FOnTimelineSelected OnTimelineSelected;
	
	//UI Getter Events
	
	//Get Global Timeline Informations
	UFUNCTION(BlueprintCallable)
	FGlobalTimelineUIInfo GetGlobalTimelineUIInformation();
	
	//Get Specific Timeline Informations 
	UFUNCTION(BlueprintCallable)
	FTimelineUIInfo GetTimelineUIInfo(int Index);
	
	//Get Current Time Key
	UFUNCTION(BlueprintCallable)
	const float& GetCurrentTimeKey() const;
	
	//Get Selected Timeline Slot
	UFUNCTION(BlueprintCallable)
	int GetSelectedTimelineSlot() const;
	
	//Get Global Timeline Length
	UFUNCTION(BlueprintCallable)
	float GetGlobalTimelineLength();
	
	//Get Recording Timeline UI Informations
	UFUNCTION(BlueprintCallable)
	FTimelineUIInfo GetRecordingTimelineUIInfo();
	
private:
	virtual void Tick(float DeltaTime) override;
	
	//Handle Replay of Player Rewind (Placement of Actions
	void PlayPlayerRewind(const float& PreviousTimeKey, const float& TimeKey);
	
	UFUNCTION()
	void OnEchoDestroyed(int EchoIndex);
protected:
	UPROPERTY()
	FGlobalTimeline GlobalTimeline;

	//Current Recording Timeline;
	UPROPERTY()
	FEchoTimeline RecordingTimeline;
	
	UPROPERTY()
	int CurrentRecordingTimelineIndex = -1;
	
	UPROPERTY()
	TObjectPtr<AActor> RecordedActor = nullptr;
	
	//Current TimeKey, used for Recording and Replays;
	UPROPERTY()
	float CurrentTimeKey = 0.0f;
	
	//Is Recording
	UPROPERTY()
	bool bIsRecording = false;
	
	//Is Timeline Replay in Rewind
	UPROPERTY()
	bool bIsInRewind = false;
	
	//Is Currently rewinding Player's Actions after a record
	UPROPERTY()
	bool bIsPlayerRewinding = false;
	
	//Current Selected Timeline Slot
	UPROPERTY()
	int SelectedSlot = 0;
	
	//Speed of Rewind (Calculated when rewind is Called
	UPROPERTY()
	float RewindSpeed = 0.f;
	
private:
	UPROPERTY()
	TObjectPtr<URecordManagerSettings> RecordManagerSettings = nullptr;
	
	UPROPERTY()
	TArray<TObjectPtr<URecordableComponent>> RecordableComponents;
	
	UPROPERTY()
	TArray<AActor*> RecordListeners;
	
	UPROPERTY()
	//Pool of EchoActor to display Timelines (avoid runtime Spawning)
	TArray<TObjectPtr<AEchoActor>> EchoActorsPool;
	
	UPROPERTY()
	//Actions to Perform on Player Rewind finish to set Echo in correct Start State (Force Grab Cube when needed to start Timeline while grabbing a cube)
	TArray<FRecordedAction> RecordingTimelineStartActions;
	
	UPROPERTY()
	float UIUpdateClock = 0.f;
};

