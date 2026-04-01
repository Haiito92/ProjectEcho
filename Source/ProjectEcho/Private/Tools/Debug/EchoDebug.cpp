// Fill out your copyright notice in the Description page of Project Settings.

#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoDebugDeveloperSettings.h"

void UEchoDebug::AddOnScreenDebugMessage(EEchoSystem SystemKey, EMessageType MessageType, const FString& Message, FLinearColor Color, float TimeToDisplay)
{
	const UEchoDebugDeveloperSettings* Settings = LazyGetDebugSettings();

	if (!Settings) return;

	const FEchoSystemDebugInfo* SystemDebugInfo = Settings->SystemDebugInfos.Find(SystemKey);
	if (!SystemDebugInfo) return;

	GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDisplay,
		Color.ToFColor(false),
		FormatMessage(SystemDebugInfo->DebugTag, MessageType,  Message)
		);
}

FString UEchoDebug::FormatMessage(const FString& Tag, const EMessageType& MessageType, const FString& Message)
{
	TArray<FStringFormatArg> Args;
	Args.Add(FStringFormatArg(Tag));
	Args.Add(FStringFormatArg(Message));
	
	const UEchoDebugDeveloperSettings* Settings = LazyGetDebugSettings();

	const FString* MessageTypeTag = nullptr;
	if (Settings) MessageTypeTag = Settings->MessageTypeTags.Find(MessageType);
	
	if (!MessageTypeTag || MessageTypeTag->IsEmpty())
	{
		return FString::Format(TEXT("[{0}]: {1}"), Args);
	}

	Args.Add(FStringFormatArg(*MessageTypeTag));
	
	return FString::Format(TEXT("[{0}][{2}]: {1}"), Args);
}

const UEchoDebugDeveloperSettings* UEchoDebug::LazyGetDebugSettings()
{
	if (DebugSettings == nullptr)
	{
		DebugSettings = GetDefault<UEchoDebugDeveloperSettings>();
	}

	return DebugSettings;
}

const UEchoDebugDeveloperSettings* UEchoDebug::DebugSettings = nullptr;

