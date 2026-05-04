// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ReflectMechanicSettings.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UReflectMechanicSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//Distance of SphereTrace from Actor's Location
	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	float SphereTraceDistance = 50.f;
	
	//SphereTrace Radius
	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	float SphereTraceRadius = 50.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Reflect")
	float ReflectPower = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Reflect")
	float ReflectCooldown = 2.f;
};
