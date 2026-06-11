// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteractMechanicSettings.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UInteractMechanicSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	float SphereTraceDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	float SphereTraceRadius = 50.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction Sphere Radius")
	float InteractionSphereRadius = 50.f;
};
