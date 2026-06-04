// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controls/PlayerActionType.h"
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
	
	ETutorialEvent Event = ETutorialEvent::Unknow;
	PlayerActionType PlayerActionType = PlayerActionType::Unknow;	
};
