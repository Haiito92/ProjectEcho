// Fill out your copyright notice in the Description page of Project Settings.


#include "Commands/CommandFunctionLibrary.h"
#include "Commands/Command.h"

void UCommandFunctionLibrary::ExecuteCommandsWithContext(TArray<TObjectPtr<UCommand>> Commands,
                                                         const FCommandContext& Context)
{
	if (Commands.IsEmpty()) return;
	
	for (TObjectPtr<UCommand> Command : Commands)
	{
		Command->ExecuteWithContext(Context);		
	}
}
