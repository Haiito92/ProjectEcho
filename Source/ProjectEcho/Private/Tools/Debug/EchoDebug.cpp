// Fill out your copyright notice in the Description page of Project Settings.

#include "Tools/Debug/EchoDebug.h"
#include "DataAssetDeveloperSettings.h"
#include "ProjectEcho.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tools/Debug/DebugDataAsset.h"
#include "Tools/Debug/EchoSystemDebugInfo.h"

void UEchoDebug::AddOnScreenDebugMessage(const EEchoSystem& SystemKey, const EEchoMessageType& MessageType, const FString& Message, const FLinearColor& Color, float TimeToDisplay)
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

void UEchoDebug::Log(const EEchoSystem& SystemKey, const EEchoMessageType& MessageType, const FString& Message)
{
	if(!IsSystemDebugActivated(SystemKey)) return;

	const FEchoSystemDebugInfo* SystemDebugInfo = GetSystemDebugInfo(SystemKey);
	if (!SystemDebugInfo) return;

	FString FinalMessage = FormatMessage(SystemDebugInfo->DebugTag, MessageType,  Message);
	
	switch (MessageType)
	{
		case EEchoMessageType::Log:
		{
			UE_LOGFMT(LogProjectEcho, Log, "{0}", FinalMessage);
			break;
		}	
		case EEchoMessageType::Warning:
		{
			UE_LOGFMT(LogProjectEcho, Warning, "{0}", FinalMessage);
			break;
		}	
		case EEchoMessageType::Error:
		{
			UE_LOGFMT(LogProjectEcho, Error, "{0}", FinalMessage);
			break;
		}
	}
}

void UEchoDebug::LogAndAddOnScreenDebugMessage(const EEchoSystem& SystemKey, const EEchoMessageType& MessageType,
	const FString& Message, const FLinearColor& Color, float TimeToDisplay)
{
	if(!IsSystemDebugActivated(SystemKey)) return;

	const FEchoSystemDebugInfo* SystemDebugInfo = GetSystemDebugInfo(SystemKey);
	if (!SystemDebugInfo) return;

	FString FinalMessage = FormatMessage(SystemDebugInfo->DebugTag, MessageType,  Message);

	GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDisplay,
		Color.ToFColor(false),
		FinalMessage
		);

	switch (MessageType)
	{
		case EEchoMessageType::Log:
		{
			UE_LOGFMT(LogProjectEcho, Log, "{0}", FinalMessage);
			break;
		}	
		case EEchoMessageType::Warning:
		{
			UE_LOGFMT(LogProjectEcho, Warning, "{0}", FinalMessage);
			break;
		}	
		case EEchoMessageType::Error:
		{
			UE_LOGFMT(LogProjectEcho, Error, "{0}", FinalMessage);
			break;
		}
	}
}

void UEchoDebug::DrawSphere(const UObject* WorldContextObject, const EEchoSystem& SystemKey, const FVector& Center, float Radius, int32 Segments, const FLinearColor& Color, float LifeTime, float Thickness, const EDrawDebugSceneDepthPriorityGroup& DepthPriority)
{
	if(!IsSystemDebugActivated(SystemKey)) return;
	
	UKismetSystemLibrary::DrawDebugSphere(WorldContextObject, Center, Radius, Segments, Color, LifeTime, Thickness, DepthPriority);
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

FString UEchoDebug::FormatMessage(const FString& Tag, const EEchoMessageType& MessageType, const FString& Message)
{
	TArray<FStringFormatArg> Args;
	Args.Add(FStringFormatArg(Tag));
	Args.Add(FStringFormatArg(Message));
	
	const UDebugDataAsset* DebugDA = LazyGetDebugDataAsset();

	const FString* MessageTypeTag = nullptr;
	if (DebugDA) MessageTypeTag = DebugDA->MessageTypeTags.Find(MessageType);
	
	if (!MessageTypeTag || MessageTypeTag->IsEmpty())
	{
		return FString::Format(TEXT("[{0}]: {1}"), Args);
	}

	Args.Add(FStringFormatArg(*MessageTypeTag));
	
	return FString::Format(TEXT("[{0}][{2}]: {1}"), Args);
}

const FEchoSystemDebugInfo* UEchoDebug::GetSystemDebugInfo(const EEchoSystem& SystemKey)
{
	const UDebugDataAsset* DebugDA = LazyGetDebugDataAsset();
	if (!DebugDA) return nullptr;

	return DebugDA->SystemDebugInfos.Find(SystemKey);
}

const UDebugDataAsset* UEchoDebug::LazyGetDebugDataAsset()
{
	if (DebugDataAsset == nullptr)
	{
		const UDataAssetDeveloperSettings* DebugSettings = GetDefault<UDataAssetDeveloperSettings>();
		DebugDataAsset = DebugSettings->DebutDataAsset.LoadSynchronous();
	}

	return DebugDataAsset;
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

TObjectPtr<UDebugDataAsset> UEchoDebug::DebugDataAsset = nullptr;
TMap<EEchoSystem, bool> UEchoDebug::Toggles = {};

