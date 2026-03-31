// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Tools/Debug/EchoSystemDebugInfo.h"
#include "EchoDebugDataAsset.generated.h"

enum EEchoSystem : uint8;
/**
 * 
 */
UCLASS()
class PROJECTECHO_API UEchoDebugDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Echo Debug")
	TMap<TEnumAsByte<EEchoSystem>, FEchoSystemDebugInfo> EchoSystemDebugInfos;
};
