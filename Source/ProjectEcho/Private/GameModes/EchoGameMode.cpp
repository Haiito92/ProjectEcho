// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/EchoGameMode.h"

#include "EchoSystem.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void AEchoGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitializeGame();
	StartGame();
}

void AEchoGameMode::InitializeGame()
{
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Initialize Game", FColor::Orange, 3.0f);
	
	URecordManagerSubsystem* RecordManagerSubsystem = GetWorld()->GetSubsystem<URecordManagerSubsystem>();

	if (RecordManagerSubsystem)
	{
		RecordManagerSubsystem->InitRecordManager(5);
	}
	
	
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
