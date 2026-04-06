// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GrabbableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGrabbableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTECHO_API IGrabbableInterface
{
	GENERATED_BODY()

public:
	//Can be Grabbed
	UFUNCTION(BLueprintNativeEvent)
	bool CanBeGrabbed();
	
	//Called when Grabbed
	UFUNCTION(BlueprintNativeEvent)
	void OnGrabbed();
};
