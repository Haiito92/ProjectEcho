// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControlDeviceType.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DeviceGameInstanceSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControlDeviceTypeChangedSignature, const ControlDeviceType&, NewControlDeviceType);

UCLASS()
class PROJECTECHO_API UDeviceGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void InitializeSubsystem();
	
	UFUNCTION(BlueprintCallable)
	const ControlDeviceType& GetCurrentControlDeviceType() const;
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InControlDeviceType"))
	void SetCurrentControlDeviceType(const ControlDeviceType& InControlDeviceType);
	
	UPROPERTY(BlueprintAssignable)
	FOnControlDeviceTypeChangedSignature OnControlDeviceTypeChanged;
	
private:
	ControlDeviceType CurrentControlDeviceType;
};
