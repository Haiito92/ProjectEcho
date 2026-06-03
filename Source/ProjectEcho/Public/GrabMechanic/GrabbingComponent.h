// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabbingRules.h"

#include "GrabbingComponent.generated.h"


struct FGrabbingRules;
class UGrabMechanicSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWillGrabActor, AActor*, GrabActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorGrabbedSignature, AActor*, GrabActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorReleasedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorThrowedSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UGrabbingComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabbingComponent();

	//Try to Grab Object in front of Player (According to Settings configurations)
	UFUNCTION(BlueprintCallable)
	virtual bool TryGrab(const FRotator& ControlRotation, const FGrabbingRules& GrabbingRules = FGrabbingRules());
	
	//Try to Release Held Object
	UFUNCTION(BlueprintCallable)
	virtual bool TryRelease();
	
	//Try to Throw Held Object
	UFUNCTION(BlueprintCallable)
	virtual bool TryThrow(const FRotator& ControlRotation);
	
	//Force Release currently held object
	UFUNCTION(BLueprintCallable)
	virtual void ForceRelease();
	
	//Force Grab given Actor (might causes issues)
	UFUNCTION(BLueprintCallable)
	virtual void ForceGrab(AActor* Actor,const FGrabbingRules& GrabbingRules = FGrabbingRules());
	
	//Try Force Grab at Holding Position (Grabs Fakely Held Cube, used for Echoes in Rewind)
	UFUNCTION(BLueprintCallable)
	virtual void TryForceGrabHeldCube(const FGrabbingRules& GrabbingRules);
	
	//Has Object Currently Grabbed
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsGrabbing();
	
	//Returns currently Grabbed Actor, return nullptr if isn't grabbing
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual AActor* GetGrabbedActor();
	
	UPROPERTY(BlueprintAssignable)
	FOnWillGrabActor OnWillGrabActor;
	
	UPROPERTY(BlueprintAssignable)
	FOnActorGrabbedSignature OnActorGrabbed;
	
	UPROPERTY(BlueprintAssignable)
	FOnActorReleasedSignature OnActorReleased;
	
	UPROPERTY(BlueprintAssignable)
	FOnActorThrowedSignature OnActorThrowed;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	//Grabbed Object;
	UPROPERTY()
	TObjectPtr<AActor> GrabbedActor = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UGrabMechanicSettings> GrabMechanicSettings = nullptr;
};
