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
	
	UPROPERTY(EditDefaultsOnly, Category="Reflect", meta=(ClampMin=0.0f, ClampMax=90.0f))
	float ReflectLiftAngle = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Reflect", meta=(ClampMin=0.0f, ClampMax=90.0f))
	float ReflectLiftAngleThreshold = 45.f;
	
	//Simulation Time of Prediction (How far the precision goes)
	UPROPERTY(EditDefaultsOnly, Category="Prediction Feedback")
	float PredictionSimulationTime = 1.0f;
	
	//Simulation Precision of Prediction (How many points are generated to make the precision)
	UPROPERTY(EditDefaultsOnly, Category="Prediction Feedback")
	float PredictionSimulationPointsFrequency = 20.0f;
	
	//Simulation Prediction Skip First points predicted before x simulated seconds
	UPROPERTY(EditDefaultsOnly, Category="Prediction Feedback")
	float PredictionSimulationSkipFirstPoints= 0.3f;
	
	//Simulation Prediction Position Offset (Starting from Push Position)
	UPROPERTY(EditDefaultsOnly, Category="Prediction Feedback")
	FVector PredictionSimulationOffset = FVector(0,0,0);
};
