// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class MenuEventType : uint8
{
	QuitPressed,
	GoToMenuPressed,
	ResumePressed,
	GoToGameLevelPressed,
	GoToMenuLevelPressed,
};
