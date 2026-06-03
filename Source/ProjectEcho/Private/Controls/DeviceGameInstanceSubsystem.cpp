// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Controls//DeviceGameInstanceSubsystem.h"

void UDeviceGameInstanceSubsystem::InitializeSubsystem()
{
	// Empty for now
}

const ControlDeviceType& UDeviceGameInstanceSubsystem::GetCurrentControlDeviceType() const
{
	return CurrentControlDeviceType;
}

void UDeviceGameInstanceSubsystem::SetCurrentControlDeviceType(const ControlDeviceType& InControlDeviceType)
{
	if (CurrentControlDeviceType == InControlDeviceType) return;
	
	CurrentControlDeviceType = InControlDeviceType;
	OnControlDeviceTypeChanged.Broadcast(CurrentControlDeviceType);
}
