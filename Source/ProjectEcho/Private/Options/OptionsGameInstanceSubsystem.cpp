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
	
	GameUserSettings = GEngine->GetGameUserSettings();
	
	if (!IsValid(GameUserSettings))
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to init options subsystem: invalid GameUserSettings");
		return;
	}
	
	WindowMode = GameUserSettings->GetFullscreenMode();
	
	AvailableWindowModes.Add(LexToString(EWindowMode::Windowed), EWindowMode::Windowed);
	AvailableWindowModes.Add(LexToString(EWindowMode::WindowedFullscreen), EWindowMode::WindowedFullscreen);
	AvailableWindowModes.Add(LexToString(EWindowMode::Fullscreen), EWindowMode::Fullscreen);
	
	ScreenResolution = GameUserSettings->GetScreenResolution();
	
	FScreenResolutionArray Resolutions;
	RHIGetAvailableResolutions(Resolutions, true);
	
	for (const FScreenResolutionRHI& ResolutionRHI : Resolutions)
	{
		FIntPoint Resolution(ResolutionRHI.Width, ResolutionRHI.Height);
		AvailableResolutions.Add(IntPointToString(Resolution), Resolution);
	}
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

FString UOptionsGameInstanceSubsystem::GetWindowModeAsString() const
{
	
	return FString(LexToString(GetWindowMode())); 
}

const FIntPoint& UOptionsGameInstanceSubsystem::GetScreenResolution() const
{
	return ScreenResolution;
}

FString UOptionsGameInstanceSubsystem::GetScreenResolutionAsString() const
{
	return IntPointToString(GetScreenResolution());
}

const TMap<FString, FIntPoint>& UOptionsGameInstanceSubsystem::GetAvailableResolutions() const
{
	return AvailableResolutions;
}

const TMap<FString, TEnumAsByte<EWindowMode::Type>>& UOptionsGameInstanceSubsystem::GetAvailableWindowModes() const
{
	return AvailableWindowModes;
}


bool UOptionsGameInstanceSubsystem::SetVolume(const OptionsVolumes& OptionVolumeType, float Volume)
{
	float ClampedVolume = FMath::Clamp(Volume, 0.0f, 1.0f);

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set volume: invalid World");
		return false;
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
	
	return true;
}

bool UOptionsGameInstanceSubsystem::SetWindowModeByString(const FString& InWindowMode)
{
	TEnumAsByte<EWindowMode::Type>* FoundWindowMode = AvailableWindowModes.Find(InWindowMode);
	
	if (!FoundWindowMode)
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set window mode by string: invalid string");
		return false;
	}
	
	return SetWindowMode(FoundWindowMode->GetValue());
}

bool UOptionsGameInstanceSubsystem::SetWindowMode(const EWindowMode::Type& InWindowMode)
{
	if (!IsValid(GameUserSettings))
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set window mode: invalid GameUserSettings");
		return false;
	}
	WindowMode = InWindowMode;
	
	GameUserSettings->SetFullscreenMode(WindowMode);
	GameUserSettings->ApplyResolutionSettings(false);
	
	return true;
}

bool UOptionsGameInstanceSubsystem::SetResolutionByString(const FString& InResolution)
{
	FIntPoint* FoundResolution = AvailableResolutions.Find(InResolution);
	
	if (!FoundResolution)
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set resolution by string: invalid string");
		return false;
	}
	
	return SetResolution(*FoundResolution);
}

bool UOptionsGameInstanceSubsystem::SetResolution(const FIntPoint& InResolution)
{
	if (!IsValid(GameUserSettings))
	{
		UEchoDebug::Log(EEchoSystem::Options, EEchoMessageType::Error, "Failed to set resolution: invalid GameUserSettings");
		return false;
	}
	
	ScreenResolution = InResolution;
	
	GameUserSettings->SetScreenResolution(ScreenResolution);
	GameUserSettings->ApplyResolutionSettings(false);
	
	return true;
}

FString UOptionsGameInstanceSubsystem::IntPointToString(const FIntPoint& InIntPoint) const
{
	return FString::FromInt(InIntPoint.X) + "x" + FString::FromInt(InIntPoint.Y);
}
