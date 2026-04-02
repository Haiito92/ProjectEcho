// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EchoSystemDebugInfo.h"
#include "MessageType.h"
#include "Engine/DeveloperSettings.h"
#include "EchoDebugDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Echo Debug Settings"))
class PROJECTECHO_API UEchoDebugDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Echo Debug Settings")
	TMap<TEnumAsByte<EMessageType>, FString> MessageTypeTags;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Echo Debug Settings")
	TMap<TEnumAsByte<EEchoSystem>, FEchoSystemDebugInfo> SystemDebugInfos;
};
