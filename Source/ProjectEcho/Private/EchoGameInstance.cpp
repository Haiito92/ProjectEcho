// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/EchoGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Options/OptionsGameInstanceSubsystem.h"

void UEchoGameInstance::Init()
{
	Super::Init();
	
	UOptionsGameInstanceSubsystem* OptionsSubsystem = GetSubsystem<UOptionsGameInstanceSubsystem>();
	
	if (IsValid(OptionsSubsystem))
	{
		OptionsSubsystem->InitializeSubsystem();
	}
}

void UEchoGameInstance::Shutdown()
{
	Super::Shutdown();
	
	UOptionsGameInstanceSubsystem* OptionsSubsystem = GetSubsystem<UOptionsGameInstanceSubsystem>();
	
	if (IsValid(OptionsSubsystem))
	{
		OptionsSubsystem->ResetResolutionToDefault();
	}
}

void UEchoGameInstance::LoadMainMenuLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
}

void UEchoGameInstance::LoadGameLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), GameLevelName);
}
