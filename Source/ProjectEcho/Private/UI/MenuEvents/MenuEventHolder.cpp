// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuEvents/MenuEventHolder.h"

#include "UI/MenuEvents/MenuEventType.h"

void UMenuEventHolder::LaunchEvent(const MenuEventType& EventType, const MenuType& GoToMenuType)
{
	switch (EventType)
	{
		case MenuEventType::PlayPressed:
			{
				OnPlayPressed.Broadcast();
				break;
			}
		case MenuEventType::QuitPressed:
			{
				OnQuitPressed.Broadcast();
				break;
			}
		case MenuEventType::GoToMenuPressed:
			{
				OnGoToMenuPressed.Broadcast(GoToMenuType);
				break;
			}
		case MenuEventType::ResumePressed:
			{
				OnResumePressed.Broadcast();
				break;
			}
		case MenuEventType::GoToLevelPressed:
			{
				OnGoToLevelPressed.Broadcast();
				break;
			}
		default:
			{
				break;
			}
	}
}
