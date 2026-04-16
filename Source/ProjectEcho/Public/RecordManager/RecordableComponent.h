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
	
	
	UFUNCTION()
	//Called to Set Actor in Rewind Mode (Has delegate for extra behaviour)
	void StartRewind();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartRewind);
	UPROPERTY(BlueprintAssignable)
	FOnStartRewind OnStartRewind;
	
	UFUNCTION()
	//Called to Reset Actor after Rewind Mode (Has delegate for extra behaviour)
	void StopRewind(const float& CurrentTimeKey);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopRewind);
	UPROPERTY(BlueprintAssignable)
	FOnStopRewind OnStopRewind;
	
	//Delegate called to warn other systems when Recordable is Interacted with
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteracted, URecordableComponent*, SelfRecordableComponent, bool, bShouldRecord);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteracted OnInteracted;
	
	UFUNCTION()
	void StartRecording(const float& CurrentTimeKey);
	
	UFUNCTION()
	//Stop Recording and Reset Keys Recorded
	void StopRecording();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsRecording() const;
	
	UFUNCTION()
	const float& GetFirstInteractedKey() const;
	
private:
	UPROPERTY()
	TArray<FRecordTransformKey> TransformKeys;
	TArray<FRecordPhysicsKey> PhysicsKeys;
	
	const FRecordTransformKey* FindPreviousTransformKey(const float& CurrentTimeKey);
	const FRecordTransformKey* FindNextTransformKey(const float& CurrentTimeKey);
	
	const FRecordPhysicsKey* FindPreviousPhysicsKey(const float& CurrentTimeKey);
	const FRecordPhysicsKey* FindNextPhysicsKey(const float& CurrentTimeKey);
	
	bool bIsRecording;
	
	//Let Component Handle Physics' Record using given Mesh
	UPROPERTY(EditDefaultsOnly)
	bool bHandlePhysicsOfMesh = false;
	
	//StaticMesh used to Handle Physics' Record
	TObjectPtr<UPrimitiveComponent> PhysicsComponent = nullptr;
	
	float FirstInteractedKey = -1;
};
