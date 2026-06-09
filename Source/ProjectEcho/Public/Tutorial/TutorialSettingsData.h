// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TutorialSettingsData.generated.h"

/**
 * 
*/

enum class EPlayerActionType : uint8;
enum class EControlDeviceType : uint8;

USTRUCT(BlueprintType)
struct FActionTutorialUIData
{
	GENERATED_BODY()
	
public:
	FActionTutorialUIData() = default;
	~FActionTutorialUIData() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText FirstText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EControlDeviceType, TObjectPtr<UTexture2D>> Icons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SecondText;
};

UCLASS(BlueprintType)
class PROJECTECHO_API UTutorialSettingsData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial UI")
	TMap<EPlayerActionType, FActionTutorialUIData> ActionTutorialsInfo;
};
