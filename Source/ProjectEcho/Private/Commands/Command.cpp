// Fill out your copyright notice in the Description page of Project Settings.


#include "Commands/Command.h"

void UCommand::Execute()
{
	ReceiveExecute();
}

void UCommand::ExecuteWithContext(const FCommandContext& Context)
{
	ReceiveExecute();
}
