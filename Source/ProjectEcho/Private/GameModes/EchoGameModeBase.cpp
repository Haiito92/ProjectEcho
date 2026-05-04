// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/EchoGameModeBase.h"
#include "EchoSystem.h"
#include "HUDs/EchoHUDBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/EchoPlayerControllerBase.h"
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
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(PlayerController))
	{
		EchoPlayerController = Cast<AEchoPlayerControllerBase>(PlayerController);
		if (IsValid(EchoPlayerController))
		{
			EchoPlayerController->InitializePlayerController();
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Successfully initialized player controller", FColor::Green, 3.0f);
			EchoPlayerController->PauseInputStarted.AddDynamic(this, &AEchoGameModeBase::OnPauseInputStarted);
		}
		else UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error, "Failed to initialize player controller", FColor::Red, 3.0f);
	}
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

void AEchoGameModeBase::ToggleGamePause()
{
	if (bIsGamePaused)
	{
		ResumeGame();
		ReceiveResumeGame();
	}
	else
	{
		PauseGame();
		ReceivePauseGame();
	}
}

void AEchoGameModeBase::PauseGame()
{
	if (bIsGamePaused) return;
	bIsGamePaused = true;
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Pause Game", FColor::Orange, 3.0f);
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	FInputModeUIOnly InputMode;
	EchoPlayerController->SetInputMode(InputMode);
	EchoPlayerController->SetShowMouseCursor(true);
	
	EchoHUD->PauseHUD();
}

void AEchoGameModeBase::ResumeGame()
{
	if (!bIsGamePaused) return;
	bIsGamePaused = false;
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Log, "Resume Game", FColor::Orange, 3.0f);
	
	FInputModeGameOnly InputMode;
	EchoPlayerController->SetInputMode(InputMode);
	EchoPlayerController->SetShowMouseCursor(false);
	
	EchoHUD->ResumeHUD();
	
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AEchoGameModeBase::OnPauseInputStarted()
{
	ToggleGamePause();
}
