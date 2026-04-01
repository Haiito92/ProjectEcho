// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EchoDebug.generated.h"

class UEchoDebugDeveloperSettings;
enum EEchoSystem : uint8;
enum EMessageType : uint8;
/** 
 * 
 */

UCLASS()
class PROJECTECHO_API UEchoDebug : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Echo Print", AutoCreateRefTerm="SystemKey,MessageType,Color"))
	static void AddOnScreenDebugMessage(const EEchoSystem& SystemKey, const EMessageType& MessageType, const FString& Message, const FLinearColor& Color, float TimeToDisplay);

private:
	static FString FormatMessage(const FString& Tag, const EMessageType& MessageType, const FString& Message);
	
	static const UEchoDebugDeveloperSettings* LazyGetDebugSettings();
	
	static const UEchoDebugDeveloperSettings* DebugSettings;
};
