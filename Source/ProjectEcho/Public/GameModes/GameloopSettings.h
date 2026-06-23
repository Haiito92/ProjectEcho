// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameloopSettings.generated.h"

class UCommand;
/**
 * 
 */
UCLASS()
class PROJECTECHO_API UGameloopSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Echo Gameloop")
	TArray<TObjectPtr<UCommand>> StartGameCommands;
};
