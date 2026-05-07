// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class LaserTargetType: uint8
{
	None = 0 UMETA(Hidden),
	Cube = 1 << 0,
	PlayerOutRecord = 1 << 1,
	PlayerInRecord = 1 << 2,
	Echo = 1 << 3,
};
ENUM_CLASS_FLAGS(LaserTargetType)
