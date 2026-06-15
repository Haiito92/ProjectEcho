// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueSystemSettings.generated.h"

/**
 * 
 */

class UCommand;

USTRUCT(Blueprintable, BlueprintType)
struct FDialogueCommandData
{
	GENERATED_BODY()
	
public:
	FDialogueCommandData() = default;
	~FDialogueCommandData() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCommand*> DialogueEndCommands;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCommand*> OnceDialogueEndCommands;

	UPROPERTY(BlueprintReadWrite)
	bool bReachEndOnce = false;
};

UCLASS()
class PROJECTECHO_API UDialogueSystemSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category= "Dialogue Commands")
	TMap<USoundBase*, FDialogueCommandData> DialogueCommands;
};
