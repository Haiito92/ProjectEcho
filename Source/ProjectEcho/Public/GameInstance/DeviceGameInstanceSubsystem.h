// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DeviceGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UDeviceGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void InitializeSubsystem ();
	
private:
};
