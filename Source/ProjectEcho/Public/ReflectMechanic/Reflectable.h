// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Reflectable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UReflectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTECHO_API IReflectable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanBeReflected() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PrepareReflect(AActor* ActorDoingReflect);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(AutoCreateRefTerm="ReflectDirection"))
	void Reflect(const FVector& ReflectDirection, float ReflectPower);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinalizeReflect();
};
