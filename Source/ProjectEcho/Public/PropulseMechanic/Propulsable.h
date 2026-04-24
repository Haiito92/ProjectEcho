// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Propulsable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPropulsable : public UInterface
{
	GENERATED_BODY()
};


class PROJECTECHO_API IPropulsable
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanBePropulsed() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PreparePropulse(AActor* PropulsingActor);
	
	UFUNCTION(blueprintNativeEvent, BlueprintCallable, meta=(AutoCreateRefTerm="PropulseDirection"))
	void Propulse(const FVector& PropulseDirection, float PropulsePower);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinalizePropulse();
};
