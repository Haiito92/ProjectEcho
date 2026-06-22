// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EchoHUDBase.generated.h"

class AEchoPlayerControllerBase;
class UMenuEventHolder;
/**
 * 
 */
UCLASS()
class PROJECTECHO_API AEchoHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:
	void InitHUD(AEchoPlayerControllerBase* InEchoPlayerController, UMenuEventHolder* InMenuEventHolder);
	void StartHUD();
	
	UFUNCTION(BlueprintCallable)
	virtual void PauseHUD();
	UFUNCTION(BlueprintCallable)
	virtual void ResumeHUD();
	
	UFUNCTION(BlueprintCallable)
	virtual void ShowEchoHUD();
	UFUNCTION(BlueprintCallable)
	virtual void HideEchoHUD();
	
private:
	virtual void InternalHUDInit(AEchoPlayerControllerBase* InEchoPlayerController, UMenuEventHolder* InMenuEventHolder);
	virtual void CreateWidgets();
	virtual void InitializeWidgets();
	
protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Internal HUD Init", ToolTip="First function called when HUD is initialized. Happens before widget creation and initialization."))
	void ReceiveInternalHUDInit(AEchoPlayerControllerBase* InEchoPlayerController, UMenuEventHolder* InMenuEventHolder);
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Create Widgets", ToolTip="Called after InternalHUDInit."))
	void ReceiveCreateWidgets();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Initialize Widgets", ToolTip="Called CreateWidgets."))
	void ReceiveInitializeWidgets();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Start HUD"))
	void ReceiveStartHUD();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Pause HUD"))
	void ReceivePauseHUD();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Resume HUD"))
	void ReceiveResumeHUD();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Show Echo HUD"))
	void ReceiveShowEchoHUD();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Receive Hide Echo HUD"))
	void ReceiveHideEchoHUD();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMenuEventHolder> MenuEventHolder;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEchoPlayerControllerBase> EchoPlayerController;
};
