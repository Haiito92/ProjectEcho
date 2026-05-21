// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OptionsSettingsData.generated.h"

class USoundSubmix;

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UOptionsSettingsData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Sound Settings")
	TObjectPtr<USoundSubmix> MasterSubmix;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound Settings")
	TObjectPtr<USoundSubmix> MusicSubmix;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound Settings")
	TObjectPtr<USoundSubmix> SFXSubmix;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound Settings")
	TObjectPtr<USoundSubmix> VoicesSubmix;
};
