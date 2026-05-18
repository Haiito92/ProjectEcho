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
	void OnBeforeGrabbed(AActor* GrabbingActor);
	
	//Called when Grabbed
	UFUNCTION(BlueprintNativeEvent)
	void OnGrabbed(AActor* GrabbingActor);
	
	//Called when Released
	UFUNCTION(BlueprintNativeEvent)
	void OnObjectReleased();
	
	//Called when Forced Released
	UFUNCTION(BlueprintNativeEvent)
	void OnObjectForceReleased();
	
	//Called when Forced Grabbed
	UFUNCTION(BlueprintNativeEvent)
	void OnObjectBeforeForceGrabbed();
	
	//Called when Forced Grabbed
	UFUNCTION(BlueprintNativeEvent)
	void OnObjectForceGrabbed(AActor* GrabbingActor);
	
	//Called when Thrown
	UFUNCTION(BlueprintNativeEvent)
	void OnThrown(const FVector& ThrowDirection,const float& ThrowForce);
};
