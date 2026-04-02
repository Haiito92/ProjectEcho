// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MessageType.generated.h"

/**
 * 
 */

UENUM()
enum EMessageType : uint8
{
	Log,
	Warning,
	Error
};
