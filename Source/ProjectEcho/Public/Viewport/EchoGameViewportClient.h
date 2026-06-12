// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "EchoGameViewportClient.generated.h"

class UDeviceGameInstanceSubsystem;
/**
 * 
 */
struct FInputKeyEventArgs;

UCLASS(Blueprintable, BlueprintType )
class PROJECTECHO_API UEchoGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
	
	virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;
	
	virtual bool InputAxis(const FInputKeyEventArgs& Args) override;
	
	virtual void ReceivedFocus(FViewport* InViewport) override;
	virtual void LostFocus(FViewport* InViewport) override;
	
	virtual TOptional<bool> QueryShowFocus(const EFocusCause InFocusCause) const override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveReceivedFocus();
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveLostFocus();
private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnViewportReceivedFocusSignature);
	UPROPERTY(BlueprintAssignable)
	FOnViewportReceivedFocusSignature OnViewportReceivedFocus;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnViewportLostFocusSignature);
	UPROPERTY(BlueprintAssignable)
	FOnViewportLostFocusSignature OnViewportLostFocus;
	
	UPROPERTY()
	TObjectPtr<UDeviceGameInstanceSubsystem> DeviceSubsystem;
};
