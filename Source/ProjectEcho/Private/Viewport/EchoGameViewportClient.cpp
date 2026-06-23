// Fill out your copyright notice in the Description page of Project Settings.


#include "Viewport/EchoGameViewportClient.h"

#include "EchoSystem.h"
#include "InputKeyEventArgs.h"
#include "Controls/DeviceGameInstanceSubsystem.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void UEchoGameViewportClient::Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance,
	bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
	
	ShouldBroadcastFocusEvents = true;
	
	DeviceSubsystem = GetGameInstance()->GetSubsystem<UDeviceGameInstanceSubsystem>();
	
	if (!IsValid(DeviceSubsystem)) UEchoDebug::Log(EEchoSystem::GameLoop, EEchoMessageType::Error, "Viewport: Device subsystem invalid");

}
bool UEchoGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	EControlDeviceType NewControlDeviceType = EControlDeviceType::KeyboardAndMouse;
	
	if (EventArgs.IsGamepad())
	{
		NewControlDeviceType = EControlDeviceType::Gamepad;
	}
	
	if (IsValid(DeviceSubsystem)) DeviceSubsystem->SetCurrentControlDeviceType(NewControlDeviceType);
	
	return Super::InputKey(EventArgs);
}

bool UEchoGameViewportClient::InputAxis(const FInputKeyEventArgs& Args)
{
	EControlDeviceType NewControlDeviceType = EControlDeviceType::KeyboardAndMouse;
	
	if (Args.IsGamepad())
	{
		NewControlDeviceType = EControlDeviceType::Gamepad;
	}
	
	if (IsValid(DeviceSubsystem)) DeviceSubsystem->SetCurrentControlDeviceType(NewControlDeviceType);
	
	return Super::InputAxis(Args);
}

void UEchoGameViewportClient::ReceivedFocus(FViewport* InViewport)
{
	Super::ReceivedFocus(InViewport);
	
	ReceiveReceivedFocus();
	
	if (ShouldBroadcastFocusEvents) OnViewportReceivedFocus.Broadcast();
}

void UEchoGameViewportClient::LostFocus(FViewport* InViewport)
{
	Super::LostFocus(InViewport);
	
	ReceiveLostFocus();
	
	if (ShouldBroadcastFocusEvents) OnViewportLostFocus.Broadcast();
}

TOptional<bool> UEchoGameViewportClient::QueryShowFocus(const EFocusCause InFocusCause) const
{
	return Super::QueryShowFocus(InFocusCause);
}

void UEchoGameViewportClient::SetShouldBroadcastFocusEvents(bool InShouldBroadcastFocusEvents)
{
	ShouldBroadcastFocusEvents = InShouldBroadcastFocusEvents;
}
