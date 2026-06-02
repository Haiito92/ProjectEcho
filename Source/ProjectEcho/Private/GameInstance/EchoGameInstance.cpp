// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameInstance/EchoGameInstance.h"

#include "EchoSystem.h"
#include "GameInstance/DeviceGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Options/OptionsGameInstanceSubsystem.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"
#include "Viewport/EchoViewport.h"

void UEchoGameInstance::Init()
{
	Super::Init();
	
	UOptionsGameInstanceSubsystem* OptionsSubsystem = GetSubsystem<UOptionsGameInstanceSubsystem>();
	
	if (IsValid(OptionsSubsystem))
	{
		OptionsSubsystem->InitializeSubsystem();
	}
	else UEchoDebug::Log(EEchoSystem::GameLoop, EEchoMessageType::Error, "Couldn't initialize OptionsSubsystem because it's invalid"); 
	
	UDeviceGameInstanceSubsystem* DeviceSubsystem = GetSubsystem<UDeviceGameInstanceSubsystem>();
	
	if (IsValid(DeviceSubsystem))
	{
		DeviceSubsystem->InitializeSubsystem();
	}
	else UEchoDebug::Log(EEchoSystem::GameLoop, EEchoMessageType::Error, "Couldn't initialize DeviceSubsystem because it's invalid"); 
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
