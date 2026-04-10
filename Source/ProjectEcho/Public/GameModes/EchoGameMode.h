// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EchoGameMode.generated.h"

class AEchoHUD;
class ACharacterST;
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
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Initialize Game")
	void ReceiveInitializeGame();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Start Game")
	void ReceiveStartGame();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive End Game")
	void ReceiveEndGame();

	TObjectPtr<ACharacterST> PlayerCharacter;
	TObjectPtr<AEchoHUD> HUD;
};
