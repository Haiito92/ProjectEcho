// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/EchoGameModeBase.h"
#include "EchoSystem.h"
#include "HUDs/EchoHUDBase.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void AEchoGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeGame();
	ReceiveInitializeGame();
	
	InitializeUI();
	ReceiveInitializeUI();
	
	StartGame();
	ReceiveStartGame();
}

void AEchoGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AEchoGameModeBase::InitializeGame()
{
}

void AEchoGameModeBase::InitializeUI()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(PlayerController))
	{
		EchoHUD = Cast<AEchoHUDBase>(PlayerController->GetHUD());

		if (IsValid(EchoHUD))
		{
			EchoHUD->InitHUD();
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Successfully initialized HUD", FColor::Green, 3.0f);
		}
		else UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error, "Failed to initialize HUD", FColor::Red, 3.0f);
	}
}



void AEchoGameModeBase::StartGame()
{
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Start Game", FColor::Orange, 3.0f);

	EchoHUD->StartHUD();
}

void AEchoGameModeBase::EndGame()
{
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "End Game", FColor::Orange, 3.0f);
}