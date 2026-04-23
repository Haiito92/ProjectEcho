// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "PropulseComponent.generated.h"


UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UPropulseComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UPropulseComponent();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	bool TryPropulse();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
	FVector DirectionalForce;
};
