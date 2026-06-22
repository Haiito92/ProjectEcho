// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEventInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameEvent: uint8
{
	Unknow = 0,
	AskEndGame
};


USTRUCT(BlueprintType)
struct PROJECTECHO_API FGameEventInfo
{
	GENERATED_BODY();
public:
	FGameEventInfo() = default;
	~FGameEventInfo() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameEvent Event = EGameEvent::Unknow;
};
