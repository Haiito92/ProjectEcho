// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Framework/Application/NavigationConfig.h"
#include "EchoGameInstance.generated.h"

/**
 * 
 */

class FEchoNavigationConfig : public FNavigationConfig
{
public:
	FEchoNavigationConfig();
};

UCLASS()
class PROJECTECHO_API UEchoGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;
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
