// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EchoDebug.generated.h"

class UDebugDataAsset;
struct FEchoSystemDebugInfo;
class UEchoDebugDeveloperSettings;
enum class EEchoSystem : uint8;
enum class EEchoMessageType : uint8;
/** 
 * 
 */

UCLASS()
class PROJECTECHO_API UEchoDebug : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Print", AutoCreateRefTerm="SystemKey,MessageType,Color", DevelopmentOnly))
	static void AddOnScreenDebugMessage(const EEchoSystem& SystemKey, const EEchoMessageType& MessageType, const FString& Message, const FLinearColor& Color = FLinearColor::White, float TimeToDisplay = 0);

	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Log", AutoCreateRefTerm="SystemKey,MessageType", DevelopmentOnly))
	static void Log(const EEchoSystem& SystemKey, const EEchoMessageType& MessageType, const FString& Message);
	
	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Log & Print", AutoCreateRefTerm="SystemKey,MessageType,Color", DevelopmentOnly))
	static void LogAndAddOnScreenDebugMessage(const EEchoSystem& SystemKey, const EEchoMessageType& MessageType, const FString& Message, const FLinearColor& Color = FLinearColor::White, float TimeToDisplay = 0.f);

	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Draw Box", WorldContext="WorldContextObject", AutoCreateRefTerm="SystemKey,Center,Extent,Rotation,LineColor,DepthPriority", DevelopmentOnly))
	static void DrawBox(const UObject* WorldContextObject, const EEchoSystem& SystemKey, const FVector& Center, const FVector& Extent, const FRotator& Rotation = FRotator::ZeroRotator, const FLinearColor& LineColor = FLinearColor::White, float Duration = 0.f, float Thickness = 0.f, const EDrawDebugSceneDepthPriorityGroup& DepthPriority = EDrawDebugSceneDepthPriorityGroup::World);

	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Draw Capsule", WorldContext="WorldContextObject", AutoCreateRefTerm="SystemKey,Center,Rotation,LineColor,DepthPriority", DevelopmentOnly))
	static void DrawCapsule(const UObject* WorldContextObject, const EEchoSystem& SystemKey, const FVector& Center, float HalfHeight, float Radius, const FRotator& Rotation = FRotator::ZeroRotator, const FLinearColor& LineColor = FLinearColor::White, float Duration = 0.f, float Thickness = 0.f, const EDrawDebugSceneDepthPriorityGroup& DepthPriority = EDrawDebugSceneDepthPriorityGroup::World);
	
	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Draw Line", WorldContext="WorldContextObject", AutoCreateRefTerm="SystemKey,LineStart,LineEnd,LineColor,DepthPriority", DevelopmentOnly))
	static void DrawLine(const UObject* WorldContextObject, const EEchoSystem& SystemKey, const FVector& LineStart, const FVector& LineEnd, const FLinearColor& LineColor = FLinearColor::White, float Duration = 0.f, float Thickness = 0.f, const EDrawDebugSceneDepthPriorityGroup& DepthPriority = EDrawDebugSceneDepthPriorityGroup::World);
	
	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Draw Sphere", WorldContext="WorldContextObject", AutoCreateRefTerm="SystemKey,Center,LineColor,DepthPriority", DevelopmentOnly))
	static void DrawSphere(const UObject* WorldContextObject, const EEchoSystem& SystemKey, const FVector& Center, float Radius = 100.f, int32 Segments = 12, const FLinearColor& LineColor = FLinearColor::White, float Duration = 0.f, float Thickness = 0.f, const EDrawDebugSceneDepthPriorityGroup& DepthPriority = EDrawDebugSceneDepthPriorityGroup::World);
	
	UFUNCTION(BlueprintCallable, Category="Echo Debug", meta=(DisplayName="Toggle System Debug", AutoCreateRefTerm="SystemKey,Activated", DevelopmentOnly))
	static void ToggleSystemDebug(const EEchoSystem& SystemKey, bool Activated);
private:
	UFUNCTION(meta=(DevelopmentOnly))
	static bool IsSystemDebugActivated(const EEchoSystem& SystemKey);
	
	UFUNCTION(meta=(DevelopmentOnly))
	static FString FormatMessage(const FString& Tag, const EEchoMessageType& MessageType, const FString& Message);

	static const FEchoSystemDebugInfo* GetSystemDebugInfo(const EEchoSystem& SystemKey);
	
	UFUNCTION(meta=(DevelopmentOnly))
	static const UDebugDataAsset* LazyGetDebugDataAsset();
	
	UFUNCTION(meta=(DevelopmentOnly))
	static TMap<EEchoSystem, bool>& LazyGetToggles();
	
	static TMap<EEchoSystem, bool> Toggles;
};
