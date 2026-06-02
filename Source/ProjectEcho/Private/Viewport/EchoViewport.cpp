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
	ControlDeviceType NewControlDeviceType = ControlDeviceType::KeyboardAndMouse;
	
	if (EventArgs.IsGamepad())
	{
		NewControlDeviceType = ControlDeviceType::Gamepad;
	}
	
	if (IsValid(DeviceSubsystem)) DeviceSubsystem->SetCurrentControlDeviceType(NewControlDeviceType);
	
	return Super::InputKey(EventArgs);
}

bool UEchoViewport::InputAxis(const FInputKeyEventArgs& Args)
{
	ControlDeviceType NewControlDeviceType = ControlDeviceType::KeyboardAndMouse;
	
	if (Args.IsGamepad())
	{
		NewControlDeviceType = ControlDeviceType::Gamepad;
	}
	
	if (IsValid(DeviceSubsystem)) DeviceSubsystem->SetCurrentControlDeviceType(NewControlDeviceType);
	
	return Super::InputAxis(Args);
}
