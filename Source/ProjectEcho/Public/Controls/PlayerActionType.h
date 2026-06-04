// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class PlayerActionType: uint8
{
	Unknow = 0,
	Move,
	Jump
};
