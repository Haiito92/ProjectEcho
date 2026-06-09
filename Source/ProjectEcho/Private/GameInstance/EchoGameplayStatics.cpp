// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/EchoGameplayStatics.h"

#include "Viewport/EchoGameViewportClient.h"

UEchoGameViewportClient* UEchoGameplayStatics::GetEchoGameViewportClient(const UObject* WorldContextObject)
{
	return Cast<UEchoGameViewportClient>(WorldContextObject->GetWorld()->GetGameViewport());
}
