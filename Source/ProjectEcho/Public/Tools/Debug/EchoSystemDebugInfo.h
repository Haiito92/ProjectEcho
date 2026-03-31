// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "EchoSystemDebugInfo.generated.h"

enum EEchoSystem : uint8;
/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTECHO_API FEchoSystemDebugInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="EchoDebug/SystemDebugInfo")
	FString DebugTag;
	
	UPROPERTY(BlueprintReadWrite, Category="EchoDebug/SystemDebugInfo")
	FColor DebugColor;
};
