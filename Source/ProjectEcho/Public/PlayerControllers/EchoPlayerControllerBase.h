// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EchoPlayerControllerBase.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class PROJECTECHO_API AEchoPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseInputStartedSignature);
	FPauseInputStartedSignature PauseInputStarted;

	void InitializePlayerController();
	
	void EnableStateMachineInputs();
	void DisableStateMachineInputs();
protected:
	void OnPauseInputStarted();
	
	UPROPERTY()
	TObjectPtr<UInputMappingContext> IMCGlobal;
	UPROPERTY()
	TObjectPtr<UInputMappingContext> IMCStateMachine;
};
