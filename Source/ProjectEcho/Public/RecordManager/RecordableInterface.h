// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RecordableInterface.generated.h"

#pragma region ActionKeys Structs

UENUM(BlueprintType)
enum class ERecordedAction : uint8
{
	None,
	Jump,
	Interact,
	TryGrab,
	TryRelease,
	TryThrow,
};

#pragma endregion
// This class does not need to be modified.
UINTERFACE()
class URecordableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTECHO_API IRecordableInterface
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
	UFUNCTION(BlueprintNativeEvent)
	TArray<ERecordedAction> GetToRecordActions();
	
	//Called to Get Control Rotation
	UFUNCTION(BlueprintNativeEvent)
	FRotator GetToRecordControlRotation();
};
