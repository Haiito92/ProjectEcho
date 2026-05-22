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
	UFUNCTION(BlueprintCallable)
	FString GetWindowModeAsString() const;
	UFUNCTION(BlueprintCallable)
	const TMap<FString, TEnumAsByte<EWindowMode::Type>>& GetAvailableWindowModes() const;
	
	UFUNCTION(BlueprintCallable)
	const FIntPoint& GetScreenResolution() const;
	UFUNCTION(BlueprintCallable)
	FString GetScreenResolutionAsString() const;
	UFUNCTION(BlueprintCallable)
	const TMap<FString, FIntPoint>& GetAvailableResolutions() const;
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="OptionVolumeType"))
	bool SetVolume(const OptionsVolumes& OptionVolumeType, float Volume);
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InWindowMode"))
	bool SetWindowModeByString(const FString& InWindowMode);
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InWindowMode"))
	bool SetWindowMode(const EWindowMode::Type& InWindowMode);
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InResolution"))
	bool SetResolutionByString(const FString& InResolution);
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InResolution"))
	bool SetResolution(const FIntPoint& InResolution);

	
	void ResetResolutionToDefault();
private:
	UFUNCTION()
	FString IntPointToString(const FIntPoint& InIntPoint) const;
	
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
	UPROPERTY()
	TMap<FString, TEnumAsByte<EWindowMode::Type>> AvailableWindowModes;
	
	UPROPERTY()
	FIntPoint ScreenResolution = FIntPoint::ZeroValue;
	UPROPERTY()
	TMap<FString, FIntPoint> AvailableResolutions;
	
	UPROPERTY()
	TObjectPtr<UGameUserSettings> GameUserSettings;
};
