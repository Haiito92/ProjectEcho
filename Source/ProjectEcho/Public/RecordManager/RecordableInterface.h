// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RecordableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
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
	UFUNCTION(BlueprintNativeEvent)
	//Called when wanting to Record a Key (CurrentTimeKey is given in global Time)
	void RecordKey(const float& CurrentTimeKey);
	
	UFUNCTION(BlueprintNativeEvent)
	//Called when wanting to Replay a Key (CurrentTimeKey is given in global Time)
	void ReplayKey(const float& PreviousTimeKey, const float& CurrentTimeKey);
	
	UFUNCTION(BlueprintNativeEvent)
	//Called to get Actor ready for Rewind (Disable Physics...)
	void StartRewind();
	
	UFUNCTION(BlueprintNativeEvent)
	//Called to warn when Record Subsystem has registered Actor
	void RegisterInRecord();
	
	UFUNCTION(BlueprintNativeEvent)
	//Called to warn when Record Subsystem has unregistered Actor
	void UnRegisterInRecord();
};
