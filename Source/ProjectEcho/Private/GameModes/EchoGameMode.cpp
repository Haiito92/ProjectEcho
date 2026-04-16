// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/EchoGameMode.h"

#include "EchoSystem.h"
#include "GameFramework/PlayerStart.h"
#include "HUDs/EchoHUD.h"
#include "Kismet/GameplayStatics.h"
#include "LevelStreaming/LevelStreamingWorldSubsystem.h"
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

	if (ULevelStreamingWorldSubsystem* LevelStreamingSubsystem = GetWorld()->GetSubsystem<ULevelStreamingWorldSubsystem>())
	{
		LevelStreamingSubsystem->InitializeLevelStreamingSubsystem();
		
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Successfully initialized Level Streaming World System", FColor::Green, 3.0f);
	}
	else UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error, "Failed to initialize Level Streaming World System", FColor::Red, 3.0f);

	if (URecordManagerSubsystem* RecordManagerSubsystem = GetWorld()->GetSubsystem<URecordManagerSubsystem>())
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

void AEchoGameMode::OnPlayerDeathEnd()
{
	if (!IsValid(EchoPlayerCharacter) || !IsValid(EchoPlayerStart)) return;

	EchoPlayerCharacter->SetActorLocation(EchoPlayerStart->GetActorLocation());

	EchoPlayerCharacter->Revive();
}
