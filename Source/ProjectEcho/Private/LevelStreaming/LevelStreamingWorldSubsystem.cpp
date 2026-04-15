// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreaming/LevelStreamingWorldSubsystem.h"
#include "EchoSystem.h"
#include "Kismet/GameplayStatics.h"
#include "LevelStreaming/StreamingLevelInfo.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void ULevelStreamingWorldSubsystem::InitializeLevelStreamingSubsystem()
{
	const TArray<ULevelStreaming*>& StreamingLevels = GetWorld()->GetStreamingLevels();

	for (int i = 0; i < StreamingLevels.Num(); i++)
	{
		ULevelStreaming* StreamLevel = StreamingLevels[i];

		FStreamingLevelInfo StreamingLevelInfo;
		StreamingLevelInfo.Index = i;
		
		StreamingLevelInfo.Path = StreamLevel->GetWorldAssetPackageName();

		FString LevelNameString = FPaths::GetBaseFilename(StreamingLevelInfo.Path);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Add stream level id for name: " + LevelNameString, FColor::Orange, 3.0f);
		LevelNameString.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		
		StreamingLevelInfo.ShortName = FName(LevelNameString);
		
		StreamLevelInfos.Add(StreamingLevelInfo);
	}
}

void ULevelStreamingWorldSubsystem::LoadStreamLevel(const FName& LevelName)
{
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Try Load Stream Level", FColor::Magenta, 3.0f);
	
	FStreamingLevelInfo* LoadedStreamLevelInfo = StreamLevelInfos.FindByPredicate([&](const FStreamingLevelInfo& StreamLevelInfo)
	{
		return StreamLevelInfo.ShortName == LevelName; 
	});
	if (!LoadedStreamLevelInfo)
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Can't load stream level: LevelStreamingInfo invalid", FColor::Red, 3.0f);
		return;
	}
	
	FLatentActionInfo Info = {};
	Info.CallbackTarget = this;
	Info.ExecutionFunction = FName("OnStreamLevelLoaded");
	Info.UUID = 0;
	Info.Linkage = LoadedStreamLevelInfo->Index;
	
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, false, Info);
}

void ULevelStreamingWorldSubsystem::UnloadStreamLevel(const FName& LevelName)
{
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Try Unload Stream Level", FColor::Magenta, 3.0f);
	
	FStreamingLevelInfo* LoadedStreamLevelInfo = StreamLevelInfos.FindByPredicate([&](const FStreamingLevelInfo& StreamLevelInfo)
	{
		return StreamLevelInfo.ShortName == LevelName; 
	});
	if (!LoadedStreamLevelInfo) return;

	const TArray<ULevelStreaming*>& StreamingLevels = GetWorld()->GetStreamingLevels();

	ULevelStreaming* StreamLevel = nullptr;
	for (int i = 0; i < StreamingLevels.Num(); i++)
	{
		if (StreamingLevels[i]->GetWorldAssetPackageName() == LoadedStreamLevelInfo->Path)
		{
			StreamLevel = StreamingLevels[i];
			break;
		}
	}

	if (!StreamLevel)
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Can't unload stream level: LevelStreaming invalid", FColor::Red, 3.0f);
		return;
	}

	ULevel* Level = StreamLevel->GetLoadedLevel();

	if (!Level)
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Can't unload stream level: Level invalid", FColor::Red, 3.0f);
		return;
	}
	
	for (const TObjectPtr<AActor> Actor : Level->Actors)
	{
		UEchoDebug::Log(EEchoSystem::GameLoop, EEchoMessageType::Log, "Stream Unloaded Actor Name: " + Actor->GetName());
	}

	FLatentActionInfo Info = {};
	Info.CallbackTarget = this;
	Info.ExecutionFunction = FName("OnStreamLevelUnloaded");
	Info.UUID = 1;
	Info.Linkage = LoadedStreamLevelInfo->Index;
	
	UGameplayStatics::UnloadStreamLevel(this, LevelName, Info, false);
}

void ULevelStreamingWorldSubsystem::OnStreamLevelLoaded(int32 Linkage)
{
	if (Linkage == INDEX_NONE) return;
	
	const FStreamingLevelInfo& LoadedStreamLevelInfo = StreamLevelInfos[Linkage];

	const TArray<ULevelStreaming*>& StreamingLevels = GetWorld()->GetStreamingLevels();

	ULevelStreaming* StreamLevel = nullptr;
	for (int i = 0; i < StreamingLevels.Num(); i++)
	{
		if (StreamingLevels[i]->GetWorldAssetPackageName() == LoadedStreamLevelInfo.Path)
		{
			StreamLevel = StreamingLevels[i];
			break;
		}
	}

	if (!StreamLevel)
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "On load stream level: LevelStreaming invalid", FColor::Red, 3.0f);
		return;
	}

	ULevel* Level = StreamLevel->GetLoadedLevel();

	if (!Level)
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "On load stream level: Level invalid", FColor::Red, 3.0f);
		return;
	}
	
	for (const TObjectPtr<AActor> Actor : Level->Actors)
	{
		UEchoDebug::Log(EEchoSystem::GameLoop, EEchoMessageType::Log, "Stream Loaded Actor Name: " + Actor->GetName());
	}
}

void ULevelStreamingWorldSubsystem::OnStreamLevelUnloaded(int32 Linkage)
{
	if (Linkage == INDEX_NONE) return;
}