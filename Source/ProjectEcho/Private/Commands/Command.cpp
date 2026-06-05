// Fill out your copyright notice in the Description page of Project Settings.


#include "Commands/Command.h"

FCommandContext::FCommandContext(AActor* InInstigator):FCommandContext(InInstigator, InInstigator->GetWorld())
{
}

FCommandContext::FCommandContext(AActor* InInstigator, UWorld* InWorld)
{
	Instigator = InInstigator;
	World = InWorld;
}

void UCommand::Execute()
{
	ExecuteWithContext({nullptr, GetWorld()});
}

void UCommand::ExecuteWithContext(const FCommandContext& Context)
{
	ReceiveExecute(Context);
}
