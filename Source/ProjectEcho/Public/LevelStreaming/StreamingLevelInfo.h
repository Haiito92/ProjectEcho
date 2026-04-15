// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StreamingLevelInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  PROJECTECHO_API FStreamingLevelInfo
{
	GENERATED_BODY()

public:
	FName ShortName;
	FString Path;
};
