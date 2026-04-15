// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/EchoGameMode.h"

#include "EchoSystem.h"
#include "GameFramework/PlayerStart.h"
#include "HUDs/EchoHUD.h"
#include "Kismet/GameplayStatics.h"
#include "LevelStreaming/StreamingLevelInfo.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "StateMachine/ACharacterST.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void AEchoGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitializeGame();
	StartGame();
}

void AEchoGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AEchoGameMode::InitializeGame()
{
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Initialize Game", FColor::Orange, 3.0f);

	
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
	
	URecordManagerSubsystem* RecordManagerSubsystem = GetWorld()->GetSubsystem<URecordManagerSubsystem>();

	if (RecordManagerSubsystem)
	{
		RecordManagerSubsystem->InitRecordManager(5);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Successfully initialized Record System", FColor::Green, 3.0f);
	}
	else UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error, "Failed to initialize Record System", FColor::Red, 3.0f);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(PlayerController))
	{
		EchoPlayerCharacter = Cast<ACharacterST>(PlayerController->GetPawn());

		if (IsValid(EchoPlayerCharacter))
		{
			EchoPlayerCharacter->InitPlayer();
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Successfully initialized Player Character", FColor::Green, 3.0f);
			EchoPlayerCharacter->OnDeathEnd.AddDynamic(this, &AEchoGameMode::OnPlayerDeathEnd);
		}
		else UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error, "Failed to initialize Player Character", FColor::Red, 3.0f);

		EchoHUD = Cast<AEchoHUD>(PlayerController->GetHUD());

		if (IsValid(EchoHUD))
		{
			EchoHUD->InitHUD();
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Successfully initialized HUD", FColor::Green, 3.0f);
		}
		else UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error, "Failed to initialize HUD", FColor::Red, 3.0f);
	}

	EchoPlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass()));
	 
	ReceiveInitializeGame();
}

void AEchoGameMode::StartGame()
{
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Start Game", FColor::Orange, 3.0f);
	ReceiveStartGame();
}

void AEchoGameMode::EndGame()
{
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "End Game", FColor::Orange, 3.0f);
	ReceiveEndGame();
}

void AEchoGameMode::LoadStreamLevel(const FName& LevelName)
{
	FStreamingLevelInfo* LoadedStreamLevelInfo = StreamLevelInfos.FindByPredicate([&](const FStreamingLevelInfo& StreamLevelInfo)
	{
		return StreamLevelInfo.ShortName == LevelName; 
	});
	if (!LoadedStreamLevelInfo) return;
	
	FLatentActionInfo Info = {};
	Info.CallbackTarget = this;
	Info.ExecutionFunction = FName("OnStreamLevelLoaded");
	Info.UUID = 0;
	Info.Linkage = LoadedStreamLevelInfo->Index;
	
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, false, Info);
}

void AEchoGameMode::UnloadStreamLevel(const FName& LevelName)
{
	FStreamingLevelInfo* LoadedStreamLevelInfo = StreamLevelInfos.FindByPredicate([&](const FStreamingLevelInfo& StreamLevelInfo)
	{
		return StreamLevelInfo.ShortName == LevelName; 
	});
	if (!LoadedStreamLevelInfo) return;
	
	FLatentActionInfo Info = {};
	Info.CallbackTarget = this;
	Info.ExecutionFunction = FName("OnStreamLevelUnloaded");
	Info.UUID = 1;
	Info.Linkage = LoadedStreamLevelInfo->Index;
	
	UGameplayStatics::UnloadStreamLevel(this, LevelName, Info, false);
}

void AEchoGameMode::OnStreamLevelLoaded(int32 Linkage)
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

	if (!StreamLevel) return;

	for (const TObjectPtr<AActor> Actor : StreamLevel->GetLoadedLevel()->Actors)
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Stream Loaded Actor Name: " + Actor->GetName(), FColor::Turquoise, 3.0f);
	}
}

void AEchoGameMode::OnStreamLevelUnloaded(int32 Linkage)
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

	if (!StreamLevel) return;

	for (const TObjectPtr<AActor> Actor : StreamLevel->GetLoadedLevel()->Actors)
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Stream Unloaded Actor Name: " + Actor->GetName(), FColor::Emerald, 3.0f);
	}
}

void AEchoGameMode::OnPlayerDeathEnd()
{
	if (!IsValid(EchoPlayerCharacter) || !IsValid(EchoPlayerStart)) return;

	EchoPlayerCharacter->SetActorLocation(EchoPlayerStart->GetActorLocation());

	EchoPlayerCharacter->Revive();
}
