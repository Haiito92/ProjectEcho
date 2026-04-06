// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GrabbingComponent.generated.h"


class UGrabMechanicSettings;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UGrabbingComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabbingComponent();

	//Try to Grab Object in front of Player (According to Settings configurations
	UFUNCTION(BlueprintCallable)
	virtual bool TryGrab(const FRotator& ControlRotation);
	
	//Has Object Currently Grabbed
	UFUNCTION(BlueprintCallable)
	virtual bool IsGrabbing();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	//Grabbed Actor;
	TObjectPtr<AActor> GrabbedActor = nullptr;

private:
	TObjectPtr<UGrabMechanicSettings> GrabMechanicSettings = nullptr;
};
