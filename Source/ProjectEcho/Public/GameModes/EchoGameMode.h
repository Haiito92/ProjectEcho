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
protected:
	virtual void InitializeGame() override;

	UFUNCTION()
	void OnPlayerDeathEnd();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerStart> EchoPlayerStart;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacterST> EchoPlayerCharacter;
};
