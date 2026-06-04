// Fill out your copyright notice in the Description page of Project Settings.


#include "Commands/CommandFunctionLibrary.h"

#include "EchoSystem.h"
#include "Commands/Command.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void UCommandFunctionLibrary::ExecuteCommandsWithContext(const TArray<UCommand*>& Commands, AActor* Instigator)
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
