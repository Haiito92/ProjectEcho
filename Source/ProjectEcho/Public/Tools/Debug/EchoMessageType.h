// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EchoMessageType.generated.h"

/**
 * 
 */

UENUM()
enum class EEchoMessageType : uint8
{
	Log,
	Warning,
	Error
};
