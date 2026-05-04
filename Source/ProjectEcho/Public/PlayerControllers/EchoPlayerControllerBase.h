// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EchoPlayerControllerBase.generated.h"

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

protected:
	virtual void SetupInputComponent() override;
	
	void OnPauseInputStarted();
};
