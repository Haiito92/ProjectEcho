// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameInstance/EchoGameInstance.h"

#include "GameInstance/DeviceGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Options/OptionsGameInstanceSubsystem.h"
#include "Viewport/EchoViewport.h"

void UEchoGameInstance::Init()
{
	Super::Init();
	
	UOptionsGameInstanceSubsystem* OptionsSubsystem = GetSubsystem<UOptionsGameInstanceSubsystem>();
	
	if (IsValid(OptionsSubsystem))
	{
		OptionsSubsystem->InitializeSubsystem();
	}
	
	UDeviceGameInstanceSubsystem* DeviceSubsystem = GetSubsystem<UDeviceGameInstanceSubsystem>();
	
	if (IsValid(DeviceSubsystem))
	{
		DeviceSubsystem->InitializeSubsystem();
	}
	
	UEchoViewport* Viewport = Cast<UEchoViewport>(GetGameViewportClient());
	
	if (IsValid(Viewport))
	{
		Viewport->InitializeViewport();
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
