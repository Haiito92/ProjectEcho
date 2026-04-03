// Fill out your copyright notice in the Description page of Project Settings.


#include "Cheats/EchoCheatManager.h"

#include "EchoSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void UEchoCheatManager::ToggleSystemDebug(EEchoSystem SystemKey, bool Activated)
{
	UEchoDebug::ToggleSystemDebug(SystemKey, Activated);
}
