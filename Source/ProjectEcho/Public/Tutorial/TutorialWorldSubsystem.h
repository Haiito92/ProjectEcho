// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TutorialWorldSubsystem.generated.h"

struct FTutorialEventInfo;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTutorialEventLaunchedSignature, const FTutorialEventInfo&, EventInfo);

UCLASS()
class PROJECTECHO_API UTutorialWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void LaunchTutorialEvent(const FTutorialEventInfo& EventInfo);
	
	UPROPERTY(BlueprintAssignable)
	FOnTutorialEventLaunchedSignature OnTutorialEventLaunched;
};
