// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controls/EPlayerActionType.h"
#include "TutorialEventInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETutorialEvent: uint8
{
	Unknow = 0,
	ShowTutorialPrompt,
	HideTutorialPrompt,
	LockInputAction,
	UnlockInputAction,
};


USTRUCT(BlueprintType)
struct PROJECTECHO_API FTutorialEventInfo
{
	GENERATED_BODY();
public:
	FTutorialEventInfo() = default;
	~FTutorialEventInfo() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETutorialEvent Event = ETutorialEvent::Unknow;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerActionType PlayerActionType = EPlayerActionType::Unknow;	
};
