// Fill out your copyright notice in the Description page of Project Settings.


#include "Cheats/EchoCheatManager.h"

#include "EchoSystem.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/MessageType.h"

void UEchoCheatManager::ToggleSystemDebug()
{
	UEchoDebug::AddOnScreenDebugMessage(
		EEchoSystem::Record,
		EMessageType::Log,
		"Toggle Record Debug",
		FColor::Magenta,
		5.0f);
}
