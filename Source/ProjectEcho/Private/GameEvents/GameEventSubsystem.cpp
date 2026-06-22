// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEvents/GameEventSubsystem.h"
#include "GameEvents/GameEventInfo.h"

void UGameEventSubsystem::InitializeEventSubsytem()
{
}

void UGameEventSubsystem::LaunchGameEvent(const FGameEventInfo& EventInfo)
{
	OnGameEventLaunched.Broadcast(EventInfo);
}
