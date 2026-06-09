// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "EchoGameplayStatics.generated.h"

class UEchoGameViewportClient;
/**
 * 
 */
UCLASS()
class PROJECTECHO_API UEchoGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Echo Game", meta=(WorldContext="WorldContextObject"))
	static UEchoGameViewportClient* GetEchoGameViewportClient(const UObject* WorldContextObject);
	
};
