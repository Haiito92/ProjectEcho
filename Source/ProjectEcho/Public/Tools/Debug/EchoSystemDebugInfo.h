// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "EchoSystemDebugInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTECHO_API FEchoSystemDebugInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Echo Debug/System Debug Info")
	FString DebugTag;
};
