// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GrabberActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UGrabberActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTECHO_API IGrabberActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//Call Force Release on Grabber Actor
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ForceRelease();
	
	//Get Mesh to attach to socket
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USkeletalMeshComponent* GetSkeletalMeshComponent();
};
