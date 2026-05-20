// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Options/OptionsEnums.h"
#include "OptionsGameInstanceSubsystem.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTECHO_API UOptionsGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void InitializeSubsystem();
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="OptionVolumeType"))
	float GetVolume(const OptionsVolumes& OptionVolumeType) const;
	
	UFUNCTION(BlueprintCallable)
	EWindowMode::Type GetWindowMode() const;
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="OptionVolumeType"))
	void SetVolume(const OptionsVolumes& OptionVolumeType, float Volume);
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InWindowMode"))
	void SetWindowMode(const EWindowMode::Type& InWindowMode);

private:
	
	UPROPERTY()
	TObjectPtr<USoundSubmix> MasterSubmix;
	UPROPERTY()
	TObjectPtr<USoundSubmix> MusicSubmix;
	UPROPERTY()
	TObjectPtr<USoundSubmix> SFXSubmix;
	UPROPERTY()
	TObjectPtr<USoundSubmix> VoicesSubmix;
	
	UPROPERTY()
	float MasterVolume = 1.0f;
	UPROPERTY()
	float MusicVolume = 1.0f;
	UPROPERTY()
	float SFXVolume = 1.0f;
	UPROPERTY()
	float VoicesVolume = 1.0f;
	
	UPROPERTY()
	TEnumAsByte<EWindowMode::Type> WindowMode = EWindowMode::WindowedFullscreen;
};
