// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EchoDebug.generated.h"

class UEchoDebugDataAsset;
enum EEchoSystem : uint8;
/**
 * 
 */

UENUM()
enum EMessageType : uint8
{
	Log,
	Warning,
	Error
};

UCLASS()
class PROJECTECHO_API UEchoDebug : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Echo Debug")
	static void AddOnScreenDebugMessage(EEchoSystem SystemKey, EMessageType MessageType, const FString& Message, float TimeToDisplay);

private:
	static const UEchoDebugDataAsset* LazyGetDebugDataAsset();
	
	static TObjectPtr<UEchoDebugDataAsset> DebugDataAsset;
};
