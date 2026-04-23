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
	
	UFUNCTION(BlueprintCallable)
	bool TryReflect(const FVector& CastStartLocation, const FVector& CastDirection);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveTryReflect();
	
private:
	UPROPERTY()
	TObjectPtr<UReflectMechanicSettings> ReflectMechanicSettings;
};
