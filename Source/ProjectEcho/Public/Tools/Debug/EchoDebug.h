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
	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Print", AutoCreateRefTerm="SystemKey,MessageType,Color"))
	static void AddOnScreenDebugMessage(const EEchoSystem& SystemKey, const EMessageType& MessageType, const FString& Message, const FLinearColor& Color, float TimeToDisplay);

	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Log", AutoCreateRefTerm="SystemKey,MessageType"))
	static void Log(const EEchoSystem& SystemKey, const EMessageType& MessageType, const FString& Message);
	
	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Toggle System Debug", AutoCreateRefTerm="SystemKey,Activated"))
	static void ToggleSystemDebug(const EEchoSystem& SystemKey, bool Activated);
private:
	static FString FormatMessage(const FString& Tag, const EMessageType& MessageType, const FString& Message);
	
	static const UEchoDebugDeveloperSettings* LazyGetDebugSettings();
	static TMap<EEchoSystem, bool>& LazyGetToggles();
	
	static const UEchoDebugDeveloperSettings* DebugSettings;
	static TMap<EEchoSystem, bool> Toggles;
};
