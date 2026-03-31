// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Debug/EchoDebug.h"

#include "Tools/Debug/EchoDebugDataAsset.h"
#include "Tools/Debug/EchoDebugDeveloperSettings.h"

void UEchoDebug::AddOnScreenDebugMessage(EEchoSystem SystemKey, EMessageType MessageType, const FString& Message, float TimeToDisplay)
{
	const UEchoDebugDataAsset* DebugData = LazyGetDebugDataAsset();

	if (!DebugData) return;

	const FEchoSystemDebugInfo* SystemDebugInfo = DebugData->EchoSystemDebugInfos.Find(SystemKey);
	if (!SystemDebugInfo) return;

	FString DebugMessage = "[" + SystemDebugInfo->DebugTag + "] " + Message;
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDisplay,
		SystemDebugInfo->DebugColor,
		DebugMessage
		);
}

const UEchoDebugDataAsset* UEchoDebug::LazyGetDebugDataAsset()
{
	if (DebugDataAsset == nullptr)
	{
		const UEchoDebugDeveloperSettings* Settings = GetDefault<UEchoDebugDeveloperSettings>();

		if (Settings != nullptr)
		{
			DebugDataAsset = Settings->DebugDataAsset.LoadSynchronous();
		}
	}

	return DebugDataAsset;
}

TObjectPtr<UEchoDebugDataAsset> UEchoDebug::DebugDataAsset = nullptr;

