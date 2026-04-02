// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EchoSystemDebugInfo.h"
#include "EchoSystem.h"
#include "MessageType.h"
#include "DebugDataAsset.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTECHO_API UDebugDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Debug Data")
	TMap<TEnumAsByte<EMessageType>, FString> MessageTypeTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Debug Data")
	TMap<TEnumAsByte<EEchoSystem>, FEchoSystemDebugInfo> SystemDebugInfos;
};
