// Fill out your copyright notice in the Description page of Project Settings.


#include "Commands/CommandFunctionLibrary.h"

#include "EchoSystem.h"
#include "Commands/Command.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void UCommandFunctionLibrary::ExecuteCommandsWithContextFromInstigator(const TArray<UCommand*>& Commands, AActor* Instigator)
{
	if (!IsValid(Instigator))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Command, EEchoMessageType::Error, "Invalid Instigator", FColor::Red, 3.0f);
		return;
	}
	
	for (TObjectPtr<UCommand> Command : Commands)
	{
		if (!IsValid(Command))
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Command, EEchoMessageType::Error, "Invalid Command Pointer", FColor::Red, 3.0f);
			continue;
		}
		Command->ExecuteWithContext({Instigator, Instigator->GetWorld()});		
	}
}

void UCommandFunctionLibrary::ExecuteCommandsWithContextFromWorld(const TArray<UCommand*>& Commands, UWorld* World)
{
	if (!IsValid(World))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Command, EEchoMessageType::Error, "Invalid World", FColor::Red, 3.0f);
		return;
	}
	
	for (TObjectPtr<UCommand> Command : Commands)
	{
		if (!IsValid(Command))
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Command, EEchoMessageType::Error, "Invalid Command Pointer", FColor::Red, 3.0f);
			continue;
		}
		Command->ExecuteWithContext({nullptr, World});		
	}
}
