// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UISettingsData.generated.h"

/**
 * 
*/

enum class PlayerActionType : uint8;
enum class EControlDeviceType : uint8;

USTRUCT(BlueprintType)
struct FInputUIData
{
	GENERATED_BODY()
	
public:
	FInputUIData() = default;
	~FInputUIData() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EControlDeviceType, TObjectPtr<UTexture2D>> Icons;
};

UCLASS(BlueprintType)
class PROJECTECHO_API UUISettingsData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs UI")
	TMap<PlayerActionType, FInputUIData> InputIcons;
};
