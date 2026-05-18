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
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void StartPropulse();
	
	UFUNCTION(BlueprintCallable)
	void StopPropulse();
	
	UFUNCTION(BlueprintCallable)
	bool TryPropulse();

	UFUNCTION(BlueprintCallable)
	void ResetCooldownTimer();
	
	UFUNCTION(BlueprintCallable)
	bool IsOn() const;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveTryPropulse();
	
private:
	UPROPERTY()
	TObjectPtr<UPropulseMechanicSettings> PropulseMechanicSettings;
	
	UPROPERTY()
	bool bIsOn;
	
	UPROPERTY()
	float PropulseCooldown;
	
	UPROPERTY()
	float PropulseTimer;
};
