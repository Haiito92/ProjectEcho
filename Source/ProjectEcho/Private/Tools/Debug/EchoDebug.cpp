// Fill out your copyright notice in the Description page of Project Settings.

#include "Tools/Debug/EchoDebug.h"
#include "EchoSystem.h"
#include "ProjectEcho.h"
#include "Tools/Debug/EchoDebugDeveloperSettings.h"

void UEchoDebug::AddOnScreenDebugMessage(const EEchoSystem& SystemKey, const EMessageType& MessageType, const FString& Message, const FLinearColor& Color, float TimeToDisplay)
{
	if(!IsSystemDebugActivated(SystemKey)) return;
	
	const FEchoSystemDebugInfo* SystemDebugInfo = GetSystemDebugInfo(SystemKey);
	if (!SystemDebugInfo) return;

	GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDisplay,
		Color.ToFColor(false),
		FormatMessage(SystemDebugInfo->DebugTag, MessageType,  Message)
		);
}

void UEchoDebug::Log(const EEchoSystem& SystemKey, const EMessageType& MessageType, const FString& Message)
{
	if(!IsSystemDebugActivated(SystemKey)) return;

	const FEchoSystemDebugInfo* SystemDebugInfo = GetSystemDebugInfo(SystemKey);
	if (!SystemDebugInfo) return;

	FString FinalMessage = FormatMessage(SystemDebugInfo->DebugTag, MessageType,  Message);
	
	switch (MessageType)
	{
		case EMessageType::Log:
		{
			UE_LOGFMT(LogProjectEcho, Log, "{0}", FinalMessage);
			break;
		}	
		case EMessageType::Warning:
		{
			UE_LOGFMT(LogProjectEcho, Warning, "{0}", FinalMessage);
			break;
		}	
		case EMessageType::Error:
		{
			UE_LOGFMT(LogProjectEcho, Error, "{0}", FinalMessage);
			break;
		}
	}
}

void UEchoDebug::ToggleSystemDebug(const EEchoSystem& SystemKey, bool Activated)
{
	auto& SystemToggles = LazyGetToggles();
	bool* SystemDebugActivated =  SystemToggles.Find(SystemKey);
	
	if (SystemDebugActivated != nullptr) *SystemDebugActivated = Activated;
}

bool UEchoDebug::IsSystemDebugActivated(const EEchoSystem& SystemKey)
{
	auto& SystemToggles = LazyGetToggles();
	bool* SystemDebugActivated =  SystemToggles.Find(SystemKey);
	if (SystemDebugActivated == nullptr) return false;

	return *SystemDebugActivated;
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

const FEchoSystemDebugInfo* UEchoDebug::GetSystemDebugInfo(const EEchoSystem& SystemKey)
{
	const UEchoDebugDeveloperSettings* Settings = LazyGetDebugSettings();
	if (!Settings) return nullptr;

	return Settings->SystemDebugInfos.Find(SystemKey);
}

const UEchoDebugDeveloperSettings* UEchoDebug::LazyGetDebugSettings()
{
	if (DebugSettings == nullptr)
	{
		DebugSettings = GetDefault<UEchoDebugDeveloperSettings>();
	}

	return DebugSettings;
}

TMap<EEchoSystem, bool>& UEchoDebug::LazyGetToggles()
{
	if (Toggles.IsEmpty())
	{
		for (EEchoSystem SystemKey : TEnumRange<EEchoSystem>())
		{
			Toggles.Add(SystemKey, false);
		}	
	}
	
	return Toggles;
}

const UEchoDebugDeveloperSettings* UEchoDebug::DebugSettings = nullptr;
TMap<EEchoSystem, bool> UEchoDebug::Toggles = {};

