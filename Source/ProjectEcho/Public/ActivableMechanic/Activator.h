// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Activator.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UActivator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTECHO_API IActivator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ToggleSwitch();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwitchOn();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwitchOff();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsSwitchOn();
};
