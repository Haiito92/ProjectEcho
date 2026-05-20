// Fill out your copyright notice in the Description page of Project Settings.


#include "Options/OptionsGameInstanceSubsystem.h"

#include "DataAssetDeveloperSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "Options/OptionsSettingsData.h"
#include "Sound/SoundSubmix.h"
#include "Tools/Debug/EchoDebug.h"

void UOptionsGameInstanceSubsystem::InitializeSubsystem()
{
	const UDataAssetDeveloperSettings* DataSettings = GetDefault<UDataAssetDeveloperSettings>();
	
	if (!IsValid(DataSettings))
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to init options subsystem: invalid DataDevSettings");
		return;
	}
	
	UOptionsSettingsData* OptionsSettingsData = DataSettings->OptionsSettings.LoadSynchronous();
	
	if (!IsValid(OptionsSettingsData))
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to init options subsystem: invalid OptionsSettingsData");
		return;
	}
	
	MasterSubmix = OptionsSettingsData->MasterSubmix;
	MusicSubmix = OptionsSettingsData->MusicSubmix;
	SFXSubmix = OptionsSettingsData->SFXSubmix;
	VoicesSubmix = OptionsSettingsData->VoicesSubmix;
}

float UOptionsGameInstanceSubsystem::GetVolume(const OptionsVolumes& OptionVolumeType) const
{
	switch (OptionVolumeType)
	{
	case OptionsVolumes::Master:
		{
			return MasterVolume;
		}
	case OptionsVolumes::Music:
		{
			return MusicVolume;
		}
	case OptionsVolumes::SFX:
		{
			return SFXVolume;
		}
	case OptionsVolumes::Voices:
		{
			return VoicesVolume;
		}
	default:
		{
			return -1.0f;
		}
	}
}

EWindowMode::Type UOptionsGameInstanceSubsystem::GetWindowMode() const
{
	return WindowMode;
}


void UOptionsGameInstanceSubsystem::SetVolume(const OptionsVolumes& OptionVolumeType, float Volume)
{
	float ClampedVolume = FMath::Clamp(Volume, 0.0f, 1.0f);

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set volume: invalid World");
		return;
	}
	
	switch (OptionVolumeType)
	{
	case OptionsVolumes::Master:
		{
			if (!IsValid(MasterSubmix))
			{
				UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set volume: invalid MasterSubmix");
				break;
			}
			
			MasterVolume = ClampedVolume;
			MasterSubmix->SetSubmixOutputVolume(GetWorld(), MasterVolume);
			break;
		}
	case OptionsVolumes::Music:
		{
			if (!IsValid(MusicSubmix))
			{
				UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set volume: invalid MusicSubmix");
				break;
			}
			
			MusicVolume = ClampedVolume;
			MusicSubmix->SetSubmixOutputVolume(GetWorld(), MusicVolume);
			break;
		}
	case OptionsVolumes::SFX:
		{
			if (!IsValid(SFXSubmix))
			{
				UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set volume: invalid SFXSubmix");
				break;
			}
			
			SFXVolume = ClampedVolume;
			SFXSubmix->SetSubmixOutputVolume(GetWorld(), SFXVolume);
			break;
		}
	case OptionsVolumes::Voices:
		{
			if (!IsValid(VoicesSubmix))
			{
				UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set volume: invalid VoicesSubmix");
				break;
			}
			
			VoicesVolume = ClampedVolume;
			VoicesSubmix->SetSubmixOutputVolume(GetWorld(), VoicesVolume);
			break;
		}
	}
}

void UOptionsGameInstanceSubsystem::SetWindowMode(const EWindowMode::Type& InWindowMode)
{
	WindowMode = InWindowMode;
	
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	
	if (!IsValid(GameUserSettings))
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set window mode: invalid GameUserSettings");
		return;
	}
	
	GameUserSettings->SetFullscreenMode(WindowMode);
	GameUserSettings->ApplyResolutionSettings(false);
}
