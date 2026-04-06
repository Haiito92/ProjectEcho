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
	
	//Called Before Being Grabbed 
	UFUNCTION(BlueprintNativeEvent)
	void OnBeforeGrabbed();
	
	//Called when Grabbed
	UFUNCTION(BlueprintNativeEvent)
	void OnGrabbed();
	
	//Called when Released
	UFUNCTION(BlueprintNativeEvent)
	void OnObjectReleased();
	
	//Called when Thrown
	UFUNCTION(BlueprintNativeEvent)
	void OnThrown(const FVector& ThrowDirection,const float& ThrowForce);
};
