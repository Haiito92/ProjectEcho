// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "StateMachine/Data/UPlayerData.h"
#include "Tools/Debug/DebugDataAsset.h"
#include "DataAssetDeveloperSettings.generated.h"

class UInteractMechanicSettings;
class UGrabMechanicSettings;
class URecordManagerSettings;
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
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Player Settings")
	TSoftObjectPtr<UPlayerData> PlayerData;
};
