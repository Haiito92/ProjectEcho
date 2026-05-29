// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RecordListener.generated.h"

enum class EEchoColor : uint8;
// This class does not need to be modified.
UINTERFACE()
class URecordListener : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTECHO_API IRecordListener
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReactToRecordStart(int TimelineIndex, EEchoColor EchoColor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReactToRecordEnd();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReactToPlayerRewindStart();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReactToPlayerRewindEnd();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReactToRewindStart();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReactToRewindEnd();
	
};
