// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/EchoGameMode.h"

#include "EchoSystem.h"
#include "HUDs/EchoHUD.h"
#include "Kismet/GameplayStatics.h"
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

void AEchoGameMode::InitializeGame()
{
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Initialize Game", FColor::Orange, 3.0f);
	
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
		PlayerCharacter = Cast<ACharacterST>(PlayerController->GetPawn());

		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->InitPlayer();
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Successfully initialized Player Character", FColor::Green, 3.0f);
		}
		else UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error, "Failed to initialize Player Character", FColor::Red, 3.0f);

		HUD = Cast<AEchoHUD>(PlayerController->GetHUD());

		if (IsValid(HUD))
		{
			HUD->InitHUD();
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Successfully initialized HUD", FColor::Green, 3.0f);
		}
		else UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error, "Failed to initialize HUD", FColor::Red, 3.0f);
		
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
