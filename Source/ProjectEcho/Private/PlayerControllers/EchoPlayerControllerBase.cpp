// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/EchoPlayerControllerBase.h"

#include "DataAssetDeveloperSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "StateMachine/Data/UInputDataConfig.h"
#include "Tools/Debug/EchoDebug.h"

void AEchoPlayerControllerBase::InitializePlayerController()
{
	const UDataAssetDeveloperSettings* DataSettings = GetDefault<UDataAssetDeveloperSettings>();
	TObjectPtr<UInputDataConfig> InputDataConfig = DataSettings->InputDataConfig.LoadSynchronous();

	if (!IsValid(InputDataConfig))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
		                                          "Failed to initialize player controller: Input Data invalid!",
		                                          FColor::Red, 3.0f);
		return;
	}

	IMCGlobal = InputDataConfig->IMCGlobal;
	IMCStateMachine = InputDataConfig->IMCStateMachine;
	
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
												  "Failed to setup input in player controller: LocalPlayer invalid!",
												  FColor::Red, 3.0f);
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	
	if (IsValid(InputSubsystem))
	{
		if (!IsValid(InputDataConfig->IMCGlobal))
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
													  "Player Controller Input Setup: IMC Global invalid!", FColor::Red,
													  3.0f);
		}
		else
		{
			InputSubsystem->AddMappingContext(IMCGlobal, 0);
		}
	
		if (!IsValid(InputDataConfig->IMCStateMachine))
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
													  "Player Controller Input Setup: IMC StateMachine invalid!", FColor::Red,
													  3.0f);
		}
		else
		{
			InputSubsystem->AddMappingContext(IMCStateMachine, 0);
		}
	}
	else
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
													  "Player Controller Input Setup: InputSystem invalid!", FColor::Red,
													  3.0f);
	}
	

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
												  "Failed to setup input in player controller: Input component invalid!",
												  FColor::Red, 3.0f);
		return;
	}
	
	if (!IsValid(InputDataConfig->APause))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
												  "Player Controller Input Setup: Pause Action invalid!", FColor::Red,
												  3.0f);
	}
	else
	{
		EnhancedInputComponent->BindAction(InputDataConfig->APause, ETriggerEvent::Started, this,
										   &AEchoPlayerControllerBase::OnPauseInputStarted);
	}
}

void AEchoPlayerControllerBase::EnableStateMachineInputs()
{
	UEnhancedInputLocalPlayerSubsystem* InputSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!IsValid(InputSystem))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
												  "Failed enable IMC: Input system invalid!",
												  FColor::Red, 3.0f);
		return;
	}

	InputSystem->AddMappingContext(IMCStateMachine, 0);
}

void AEchoPlayerControllerBase::DisableStateMachineInputs()
{
	UEnhancedInputLocalPlayerSubsystem* InputSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!IsValid(InputSystem))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::GameLoop, EEchoMessageType::Error,
												  "Failed enable IMC: Input system invalid!",
												  FColor::Red, 3.0f);
		return;
	}

	InputSystem->RemoveMappingContext(IMCStateMachine);
}

void AEchoPlayerControllerBase::OnPauseInputStarted()
{
	PauseInputStarted.Broadcast();
}
