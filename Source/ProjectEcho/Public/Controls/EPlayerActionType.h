// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/EnumRange.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerActionType: uint8
{
	Unknow = 0,
	Move,
	Jump,
	Interact,
	GrabOrRelease,
	Throw, 
	Reflect,
	Record,
	SelectTimeline,
	DeleteTimeline,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EPlayerActionType, EPlayerActionType::Count);
