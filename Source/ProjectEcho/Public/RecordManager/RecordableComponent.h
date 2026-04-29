// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecordKeysStructs.h"

#include "RecordableComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API URecordableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URecordableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	//Record a TransformKey at CurrentTimeKey (given in global Timeline) for Owner
	UFUNCTION()
	void RecordKey(const float& CurrentTimeKey);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecordKey, const float&, CurrentTimeKey);
	UPROPERTY(BlueprintAssignable)
	FOnRecordKey OnRecordKey;
	
	UFUNCTION()
	//Replay Current Transform Key (given in global Timeline) in Rewind for Owner
	void ReplayKey(const float& PreviousTimeKey, const float& CurrentTimeKey);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReplayKey, const float&, PreviousTimeKey, const float&, CurrentTimeKey);
	UPROPERTY(BlueprintAssignable)
	FOnReplayKey OnReplayKey;
	
	UFUNCTION()
	void ReplayFirstKey();
	
	UFUNCTION(BlueprintCallable)
	void MarkAsCurrentlyInteracted();
	
	UFUNCTION(BlueprintCallable)
	void UnmarkAsCurrentlyInteracted();
	
	UFUNCTION()
	//Called to Set Actor in Rewind Mode (Has delegate for extra behaviour)
	void StartRewind();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartRewind);
	UPROPERTY(BlueprintAssignable)
	FOnStartRewind OnStartRewind;
	
	UFUNCTION()
	//Called to Reset Actor after Rewind Mode (Has delegate for extra behaviour)
	void StopRewind(const float& CurrentTimeKey, bool bForceReset = false);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopRewind);
	UPROPERTY(BlueprintAssignable)
	FOnStopRewind OnStopRewind;
	
	//Delegate called to warn other systems when Recordable is Interacted with
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInteracted, URecordableComponent*, SelfRecordableComponent, bool, bShouldRecord, int, RecordTimelineIndex);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteracted OnInteracted;
	
	UFUNCTION()
	void StartRecording(const FRecordInteractionKey& FirstInteractionKey);
	
	UFUNCTION()
	void RegisterInteractionKey(const FRecordInteractionKey& InteractionKey);
	
	UFUNCTION()
	void HandleTimelineDestruction(const int& RecordTimelineIndex);
	
	UFUNCTION()
	//Stop Recording and Reset Keys Recorded
	void StopRecording(bool bForceStopRecording = false);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsRecording() const;
	
	//Is Currently Interacted With (Can be Used to Start Record)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCurrentlyInteractedWith() const;
	
	UFUNCTION()
	float GetFirstInteractedKey() const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsInteractedWith = false; 
	
private:
	UPROPERTY()
	TArray<FRecordTransformKey> TransformKeys;
	
	UPROPERTY()
	TArray<FRecordPhysicsKey> PhysicsKeys;
	
	UPROPERTY()
	TArray<FRecordInteractionKey> InteractionKeys;
	
	//Clear All Keys Coming after Current Key
	void ClearKeysPastCurrentKey(const float& CurrentTimeKey);
	
	//Clear All Keys Coming before Current Key
	void ClearKeysBeforeCurrentKey(const float& CurrentTimeKey);
	
	const FRecordTransformKey* FindPreviousTransformKey(const float& CurrentTimeKey);
	const FRecordTransformKey* FindNextTransformKey(const float& CurrentTimeKey);
	
	const FRecordPhysicsKey* FindPreviousPhysicsKey(const float& CurrentTimeKey);
	const FRecordPhysicsKey* FindNextPhysicsKey(const float& CurrentTimeKey);
	
	bool bIsRecording = false;
	
	//When true, will not simulate physics on Stop Rewind
	bool bStartInteracted = false;
	
	//Let Component Handle Physics' Record using given Mesh
	UPROPERTY(EditDefaultsOnly)
	bool bHandlePhysicsOfMesh = false;
	
	//StaticMesh used to Handle Physics' Record
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> PhysicsComponent = nullptr;
};
