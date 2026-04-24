// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "PropulseComponent.generated.h"


class UPropulseMechanicSettings;

UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UPropulseComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UPropulseComponent();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	bool TryPropulse();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveTryPropulse();
	
private:
	UPROPERTY()
	TObjectPtr<UPropulseMechanicSettings> PropulseMechanicSettings;
};
