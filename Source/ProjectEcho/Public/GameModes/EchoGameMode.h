// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EchoGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTECHO_API AEchoGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
	void InitializeGame();
	void StartGame();
	void EndGame();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveInitializeGame();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveStartGame();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveEndGame();
};
