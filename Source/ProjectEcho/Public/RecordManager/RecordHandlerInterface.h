// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RecordHandlerInterface.generated.h"

#pragma region ActionKeys Structs

#pragma endregion
struct FRecordAnimationKey;
struct FRecordedAction;
// This class does not need to be modified.
UINTERFACE()
class URecordHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTECHO_API IRecordHandlerInterface
{
	GENERATED_BODY()
public:
	//Called when should Start Recording
	UFUNCTION(BlueprintNativeEvent)
	void StartRecording();
	
	//Called when should Stop Recording
	UFUNCTION(BlueprintNativeEvent)
	void StopRecording();
	
	//Called to get RecordedAction List
	UFUNCTION()
	virtual TArray<FRecordedAction> GetToRecordActions() = 0;
	
	//Called to get Rewind RecordedAction List
	UFUNCTION()
	virtual TArray<FRecordedAction> GetToRecordRewindActions() = 0;
	
	//Called to get AnimationsKeys
	UFUNCTION()
	virtual TArray<FRecordAnimationKey> GetToRecordAnimationKeys() = 0;
	
	//Called to Get Control Rotation
	UFUNCTION(BlueprintNativeEvent)
	FRotator GetToRecordControlRotation();
	
	//Called to Set Control Rotation during a Rewind
	UFUNCTION(BlueprintNativeEvent)
	void SetControlRotation(FRotator NewControlRotation);
	
	//Called to Handle Rewinds Action (Restore State to before Record)
	UFUNCTION(BlueprintNativeEvent)
	void HandleRewindActionKey(const FRecordedAction& RecordedAction);
};
