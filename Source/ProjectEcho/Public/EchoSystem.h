// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EchoSystem.generated.h"

/**
 * 
 */
UENUM()
enum class EEchoSystem : uint8
{
	PlayerStateMachine,
	Record,
	Grab,
	Activable,
	Interact,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EEchoSystem, EEchoSystem::Count)
