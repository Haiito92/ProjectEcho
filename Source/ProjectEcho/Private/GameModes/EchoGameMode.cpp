// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/EchoGameMode.h"

void AEchoGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitializeGame();
}

void AEchoGameMode::InitializeGame()
{
	ReceiveInitializeGame();
}

void AEchoGameMode::StartGame()
{
	ReceiveStartGame();
}

void AEchoGameMode::EndGame()
{
	ReceiveEndGame();
}
