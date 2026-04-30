// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EchoGameModeBase.h"
#include "GameFramework/GameModeBase.h"
#include "EchoGameMode.generated.h"

class AEchoHUD;
class ACharacterST;
class APlayerStart;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTECHO_API AEchoGameMode : public AEchoGameModeBase
{
	GENERATED_BODY()
private:
	virtual void InitializeGame() override;

protected:
	
	UFUNCTION()
	void OnPlayerDeathEnd();
	
	UPROPERTY()
	TObjectPtr<APlayerStart> EchoPlayerStart;
	UPROPERTY()
	TObjectPtr<ACharacterST> EchoPlayerCharacter;
};
