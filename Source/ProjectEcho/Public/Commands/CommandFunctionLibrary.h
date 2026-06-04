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
	static void ExecuteCommandsWithContext(TArray<TObjectPtr<UCommand>> Commands, const FCommandContext& Context);
};
