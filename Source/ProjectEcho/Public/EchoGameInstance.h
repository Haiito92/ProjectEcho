// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EchoGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UEchoGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void LoadMainMenuLevel();
	
	UFUNCTION(BlueprintCallable)
	void LoadGameLevel();
	
	UPROPERTY(EditAnywhere)
	FName MainMenuLevelName;
	
	UPROPERTY(EditAnywhere)
	FName GameLevelName;
};
