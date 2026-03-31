// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Debug/EchoDebug.h"

void UEchoDebug::AddOnScreenDebugMessage(EEchoSystem SystemKey, EMessageType MessageType, const FString& Message, float TimeToDisplay)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDisplay,
		FColor::Cyan,
		Message
		);
}
