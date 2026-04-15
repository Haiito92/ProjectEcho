// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelStreamingWorldSubsystem.generated.h"

struct FStreamingLevelInfo;

/**
 * 
 */
UCLASS()
class PROJECTECHO_API ULevelStreamingWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void InitializeLevelStreamingSubsystem();
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="LevelName"))
	void LoadStreamLevel(const FName& LevelName);

	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="LevelName"))
	void UnloadStreamLevel(const FName& LevelName);
	
protected:
	UFUNCTION()
	void OnStreamLevelLoaded(int32 Linkage);
	
	UFUNCTION()
	void OnStreamLevelUnloaded(int32 Linkage);

private:
	UPROPERTY()
	TArray<FStreamingLevelInfo> StreamLevelInfos;
};
