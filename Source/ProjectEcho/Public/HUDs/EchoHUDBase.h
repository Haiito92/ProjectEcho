// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EchoHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API AEchoHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:
	void InitHUD();
	
private:
	virtual void InternalHUDInit();
	virtual void CreateWidgets();
	virtual void InitializeWidgets();
	
protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Internal HUD Init", ToolTip="First function called when HUD is initialized. Happens before widget creation and initialization."))
	void ReceiveInternalHUDInit();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Create Widgets", ToolTip="Called after InternalHUDInit."))
	void ReceiveCreateWidgets();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Initialize Widgets", ToolTip="Called CreateWidgets."))
	void ReceiveInitializeWidgets();
};
