// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PropulseMechanicSettings.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UPropulseMechanicSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category="Propulse")
	float PropulsePower = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Propulse")
	float PropulseCooldown = 2.f;
};
