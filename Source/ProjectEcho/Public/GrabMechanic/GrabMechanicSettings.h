// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GrabMechanicSettings.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UGrabMechanicSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//Offset of SphereTrace from Actor's Location
	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	FVector SphereTraceBaseLocationOffset = FVector(0.0f, 0.0f, 0.0f);
	
	//Distance of SphereTrace from Actor's Location
	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	float SphereTraceDistance = 50.f;
	
	//SphereTrace Radius
	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	float SphereTraceRadius = 50.f;
	
	//Strength of the Throw Force added to the Object
	UPROPERTY(EditDefaultsOnly, Category="Throw")
	float ThrowStrength = 5.0f;
};
