// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/EchoPlayerControllerBase.h"

#include "DataAssetDeveloperSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "StateMachine/Data/UInputDataConfig.h"
#include "Tools/Debug/EchoDebug.h"

void AEchoPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	const UDataAssetDeveloperSettings* DataSettings = GetDefault<UDataAssetDeveloperSettings>();
	if (!IsValid(DataSettings))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
		                                          "Failed to setup input in player controller: DataSettings invalid!",
		                                          FColor::Red, 3.0f);
		return;
	}

	TObjectPtr<UInputDataConfig> InputDataConfig = DataSettings->InputDataConfig.LoadSynchronous();
	if (!IsValid(InputDataConfig))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
		                                          "Failed to setup input in player controller: Input Data invalid!",
		                                          FColor::Red, 3.0f);
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
		                                          "Failed to setup input in player controller: LocalPlayer invalid!",
		                                          FColor::Red, 3.0f);
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,"Failed to setup input in player controller: Input component invalid!", FColor::Red, 3.0f);
		return;
	}

	if (!IsValid(InputDataConfig->APause))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,"Player Controller Input Setup: Pause Action invalid!", FColor::Red, 3.0f);
	}
	else
	{
		EnhancedInputComponent->BindAction(InputDataConfig->APause, ETriggerEvent::Started, this, &AEchoPlayerControllerBase::OnPauseInputStarted);
	}
}

void AEchoPlayerControllerBase::OnPauseInputStarted()
{
	PauseInputStarted.Broadcast();
}
