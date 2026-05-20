// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReflectComponent.generated.h"


class UReflectMechanicSettings;

UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UReflectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UReflectComponent();
	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InCastStartLocation, InCastDirection"))
	void StartReflect(const FVector& InCastStartLocation, const FVector& InCastDirection);
	
	void StartReflect();
	
	UFUNCTION(BlueprintCallable)
	void StopReflect();
	
	UFUNCTION(BlueprintCallable)
	void ResetCooldownTimer();
	
	UFUNCTION(BlueprintCallable)
	bool IsOn() const;
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InCastStartLocation"))
	void SetCastStartLocation(const FVector& InCastStartLocation);
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InCastDirection"))
	void SetCastDirection(const FVector& InCastDirection);
	
protected:
	UFUNCTION(BlueprintCallable)
	bool TryReflect();
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveTryReflect();
	
private:
	UPROPERTY()
	TObjectPtr<UReflectMechanicSettings> ReflectMechanicSettings;
	
	UPROPERTY()
	bool bIsOn;
	
	UPROPERTY()
	float ReflectCooldown;
	
	UPROPERTY()
	float ReflectTimer;
	
	UPROPERTY()
	FVector CastStartLocation;
	UPROPERTY()
	FVector CastDirection;
};
