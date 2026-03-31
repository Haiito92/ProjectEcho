// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "EchoDebugDeveloperSettings.generated.h"

class UEchoDebugDataAsset;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Echo Debug Settings"))
class PROJECTECHO_API UEchoDebugDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Echo Debug Settings")
	TSoftObjectPtr<UEchoDebugDataAsset> DebugDataAsset;
};
