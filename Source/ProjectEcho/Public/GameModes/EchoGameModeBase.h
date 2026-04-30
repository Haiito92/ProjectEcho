// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EchoGameModeBase.generated.h"

class AEchoHUD;
/**
 * 
 */
UCLASS()
class PROJECTECHO_API AEchoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	virtual void InitializeGame();
	virtual void InitializeUI();
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Initialize Game")
	void ReceiveInitializeGame();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Initialize UI")
	void ReceiveInitializeUI();
	
	UPROPERTY()
	TObjectPtr<AEchoHUD> EchoHUD;
};
