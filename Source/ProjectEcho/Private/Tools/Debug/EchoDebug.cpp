// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Debug/EchoDebug.h"

#include "Tools/Debug/EchoDebugDataAsset.h"
#include "Tools/Debug/EchoDebugDeveloperSettings.h"

void UEchoDebug::AddOnScreenDebugMessage(EEchoSystem SystemKey, EMessageType MessageType, const FString& Message, float TimeToDisplay)
{
	const UEchoDebugDeveloperSettings* Settings = GetDefault<UEchoDebugDeveloperSettings>();

	UEchoDebugDataAsset* DebugData = Settings->DebugDataAsset.LoadSynchronous();

	if (!DebugData) return;

	FEchoSystemDebugInfo* SystemDebugInfo = DebugData->EchoSystemDebugInfos.Find(SystemKey);
	if (!SystemDebugInfo) return;

	GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDisplay,
		SystemDebugInfo->DebugColor,
		Message
		);
}
