// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ReflectMechanicSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPredictParams
{
	GENERATED_BODY()
	
	//Simulation Time of Prediction (How far the precision goes)
	UPROPERTY(EditDefaultsOnly)
	float SimulationTime = 1.f;
	
	//Simulation Precision of Prediction (How many points are generated to make the precision)
	UPROPERTY(EditDefaultsOnly)
	float SimulationPointsFrequency = 20.0f;
	
	//Simulation Prediction Skip First points predicted before x simulated seconds
	UPROPERTY(EditDefaultsOnly)
	float SimulationSkipFirstPoints = 0.3f;
};

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
	
	//Prediction Gravity Overriding Value (Used to simulate friction of air)
	UPROPERTY(EditDefaultsOnly, Category="Prediction Feedback")
	float PredictionOverrideGravityForce = 0.f;
	
	//Parameters used for Prediction of Reflect with Player (First Person)
	UPROPERTY(EditDefaultsOnly, Category="Prediction Feedback")
	FPredictParams PlayerPredictionParameters = FPredictParams();
	
	//Parameters used for Prediction of Reflect with Echo (Third Person)
	UPROPERTY(EditDefaultsOnly, Category="Prediction Feedback")
	FPredictParams EchoPredictionParameters = FPredictParams();
};
