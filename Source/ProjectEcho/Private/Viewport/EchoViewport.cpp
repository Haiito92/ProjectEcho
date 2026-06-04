// Fill out your copyright notice in the Description page of Project Settings.


#include "Viewport/EchoViewport.h"

#include "EchoSystem.h"
#include "InputKeyEventArgs.h"
#include "Controls/DeviceGameInstanceSubsystem.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void UEchoViewport::Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance,
	bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
	
	DeviceSubsystem = GetGameInstance()->GetSubsystem<UDeviceGameInstanceSubsystem>();
	
	if (!IsValid(DeviceSubsystem)) UEchoDebug::Log(EEchoSystem::GameLoop, EEchoMessageType::Error, "Viewport: Device subsystem invalid");

}
bool UEchoViewport::InputKey(const FInputKeyEventArgs& EventArgs)
{
	EControlDeviceType NewControlDeviceType = EControlDeviceType::KeyboardAndMouse;
	
	if (EventArgs.IsGamepad())
	{
		NewControlDeviceType = EControlDeviceType::Gamepad;
	}
	
	if (IsValid(DeviceSubsystem)) DeviceSubsystem->SetCurrentControlDeviceType(NewControlDeviceType);
	
	return Super::InputKey(EventArgs);
}

bool UEchoViewport::InputAxis(const FInputKeyEventArgs& Args)
{
	EControlDeviceType NewControlDeviceType = EControlDeviceType::KeyboardAndMouse;
	
	if (Args.IsGamepad())
	{
		NewControlDeviceType = EControlDeviceType::Gamepad;
	}
	
	if (IsValid(DeviceSubsystem)) DeviceSubsystem->SetCurrentControlDeviceType(NewControlDeviceType);
	
	return Super::InputAxis(Args);
}
