// Fill out your copyright notice in the Description page of Project Settings.


#include "Viewport/EchoViewport.h"

void UEchoViewport::InitializeViewport()
{
	
}

bool UEchoViewport::InputKey(const FInputKeyEventArgs& EventArgs)
{
	return Super::InputKey(EventArgs);
	
	if (EventArgs.IsGamepad())
	{
		// Set device to gamepad
	}
	else
	{
		// Set to keyboard
	}
	
}

bool UEchoViewport::InputAxis(const FInputKeyEventArgs& Args)
{
	return Super::InputAxis(Args);
	
	if (Args.IsGamepad())
	{
		// Set device to gamepad
	}
	else
	{
		// Set to keyboard
	}
}
