// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "StateMachine/Data/UPlayerData.h"
#include "Tools/Debug/DebugDataAsset.h"
#include "DataAssetDeveloperSettings.generated.h"

class UTutorialSettingsData;
class UOptionsSettingsData;
class UInputDataConfig;
class UPropulseMechanicSettings;
class UReflectMechanicSettings;
class UInteractMechanicSettings;
class UGrabMechanicSettings;
class URecordManagerSettings;
class UStateMachineSettings;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Data Asset Settings"))
class PROJECTECHO_API UDataAssetDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//Data Asset of Debug Domain Tool Settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Echo Debug Settings")
	TSoftObjectPtr<UDebugDataAsset> DebutDataAsset;
	
	//Data Asset of Record Mechanics Settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Record Settings")
	TSoftObjectPtr<URecordManagerSettings> RecordManagerSettings;
	
	//Data Asset of Grab Mechanic Settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Grab Settings")
	TSoftObjectPtr<UGrabMechanicSettings> GrabMechanicSettings;

	//Data Asset of Interact Mechanic Settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Interact Settings")
	TSoftObjectPtr<UInteractMechanicSettings> InteractMechanicSettings;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Reflect Settings")
	TSoftObjectPtr<UReflectMechanicSettings> ReflectMechanicSettings;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Propulse Settings")
	TSoftObjectPtr<UPropulseMechanicSettings> PropulseMechanicSettings;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Player Settings")
	TSoftObjectPtr<UPlayerData> PlayerData;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="States Settings")
	TSoftObjectPtr<UStateMachineSettings> StateMachineData;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Input Settings")
	TSoftObjectPtr<UInputDataConfig> InputDataConfig;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Options Settings")
	TSoftObjectPtr<UOptionsSettingsData> OptionsSettings;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="UI Settings")
	TSoftObjectPtr<UTutorialSettingsData> UISettings;
};
