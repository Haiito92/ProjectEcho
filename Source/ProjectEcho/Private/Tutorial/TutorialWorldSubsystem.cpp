// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/TutorialWorldSubsystem.h"

void UTutorialWorldSubsystem::LaunchTutorialEvent(const FTutorialEventInfo& EventInfo)
{
	OnTutorialEventLaunched.Broadcast(EventInfo);
}
