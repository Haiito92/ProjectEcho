// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommandFunctionLibrary.generated.h"

struct FCommandContext;
class UCommand;
/**
 * 
 */
UCLASS()
class PROJECTECHO_API UCommandFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void ExecuteCommandsWithContextFromInstigator(const TArray<UCommand*>& Commands, AActor* Instigator);
	
	UFUNCTION(BlueprintCallable)
	static void ExecuteCommandsWithContextFromWorld(const TArray<UCommand*>& Commands, UWorld* World);
};
