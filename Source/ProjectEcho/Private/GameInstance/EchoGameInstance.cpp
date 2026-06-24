// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameInstance/EchoGameInstance.h"

#include "EchoSystem.h"
#include "Controls/DeviceGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Options/OptionsGameInstanceSubsystem.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"
#include "Viewport/EchoGameViewportClient.h"

FEchoNavigationConfig::FEchoNavigationConfig()
{
	KeyEventRules.Emplace(EKeys::Z, EUINavigation::Up);
	KeyEventRules.Emplace(EKeys::S, EUINavigation::Down);
	KeyEventRules.Emplace(EKeys::Q, EUINavigation::Left);
	KeyEventRules.Emplace(EKeys::D, EUINavigation::Right);
}

void UEchoGameInstance::Init()
{
	Super::Init();
	
	GEngine->SetMaxFPS(144);
	
	TSharedRef<FEchoNavigationConfig> Config = MakeShared<FEchoNavigationConfig>();
	FSlateApplication::Get().SetNavigationConfig(Config);
	
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
