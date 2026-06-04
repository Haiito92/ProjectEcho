// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EControlDeviceType.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DeviceGameInstanceSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControlDeviceTypeChangedSignature, const EControlDeviceType&, NewControlDeviceType);

UCLASS()
class PROJECTECHO_API UDeviceGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void InitializeSubsystem();
	
	UFUNCTION(BlueprintCallable)
	const EControlDeviceType& GetCurrentControlDeviceType() const;
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="InControlDeviceType"))
	void SetCurrentControlDeviceType(const EControlDeviceType& InControlDeviceType);
	
	UPROPERTY(BlueprintAssignable)
	FOnControlDeviceTypeChangedSignature OnControlDeviceTypeChanged;
	
private:
	EControlDeviceType CurrentControlDeviceType;
};
