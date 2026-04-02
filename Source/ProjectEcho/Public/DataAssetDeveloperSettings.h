// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DataAssetDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Data Asset Settings"))
class PROJECTECHO_API UDataAssetDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
};
