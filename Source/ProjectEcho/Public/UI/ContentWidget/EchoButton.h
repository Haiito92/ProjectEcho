// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "EchoButton.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonReceivedFocusEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonLostFocusEvent);

UCLASS()
class PROJECTECHO_API UEchoButton : public UButton
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Button|Event")
	FOnButtonReceivedFocusEvent OnReceivedFocus;

	UPROPERTY(BlueprintAssignable, Category="Button|Event")
	FOnButtonLostFocusEvent OnLostFocus; 
	
protected:
	void EchoSlateHandleOnReceivedFocus();
	void EchoSlateHandleOnLostFocus();
	
	UFUNCTION(BlueprintNativeEvent)
	void ReceivedFocus();
	UFUNCTION(BlueprintNativeEvent)
	void LostFocus();
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
};
