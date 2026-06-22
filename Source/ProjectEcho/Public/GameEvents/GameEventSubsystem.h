// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameEventSubsystem.generated.h"

struct FGameEventInfo;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEventLaunchedSignature, const FGameEventInfo&, EventInfo);

UCLASS()
class PROJECTECHO_API UGameEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeEventSubsytem();
	
	UFUNCTION(BlueprintCallable)
	void LaunchGameEvent(const FGameEventInfo& EventInfo);
	
	UPROPERTY(BlueprintAssignable)
	FOnGameEventLaunchedSignature OnGameEventLaunched;
};
