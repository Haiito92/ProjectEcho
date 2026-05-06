// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/EchoGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UEchoGameInstance::LoadMainMenuLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
}

void UEchoGameInstance::LoadGameLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), GameLevelName);
}
