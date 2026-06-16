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
	//Offset of SphereCast Start Location from Actor's Location
	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	FVector SphereTraceBaseLocationOffset = FVector(0.0f, 0.0f, 0.0f);
	
	//Length of SphereCast (Distance between Start of Cast and End of Cast)
	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	float SphereTraceDistance = 50.f;
	
	//SphereCast Radius
	UPROPERTY(EditDefaultsOnly, Category="Sphere Trace")
	float SphereTraceRadius = 50.f;
	
	//Strength of the Throw Force added to the Object
	UPROPERTY(EditDefaultsOnly, Category="Throw")
	float ThrowStrength = 5.0f;
	
	//Radius at which cube is supposed to be interactable (Anamorphose will be full)
	UPROPERTY(EditDefaultsOnly, Category="Anamorphose")
	float AnamorphoseFullInteractionRadius = 150.f;
};
