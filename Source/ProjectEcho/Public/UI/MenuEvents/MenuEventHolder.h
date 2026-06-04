// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MenuEventHolder.generated.h"

enum class MenuType : uint8;
enum class MenuEventType : uint8;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoToMenuPressedSignature, const MenuType&, GoToMenuType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResumePressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoToMenuLevelPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoToGameLevelPressedSignature);

UCLASS(BlueprintType, Blueprintable)
class PROJECTECHO_API UMenuEventHolder : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnQuitPressedSignature OnQuitPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnGoToMenuPressedSignature OnGoToMenuPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnResumePressedSignature OnResumePressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnGoToMenuLevelPressedSignature OnGoToMenuLevelPressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnGoToGameLevelPressedSignature OnGoToGameLevelPressed;
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="EventType,GoToMenuType"))
	void LaunchEvent(const MenuEventType& EventType, const MenuType& GoToMenuType);
};
