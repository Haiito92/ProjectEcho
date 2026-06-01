// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/EchoHUDBase.h"
#include "UI/MenuEvents/MenuEventHolder.h"

void AEchoHUDBase::InitHUD(AEchoPlayerControllerBase* InEchoPlayerController, UMenuEventHolder* InMenuEventHolder)
{
	InternalHUDInit(InEchoPlayerController, InMenuEventHolder);
	
	CreateWidgets();
	
	InitializeWidgets();
}

void AEchoHUDBase::StartHUD()
{
	ReceiveStartHUD();
}

void AEchoHUDBase::PauseHUD()
{
	ReceivePauseHUD();
}

void AEchoHUDBase::ResumeHUD()
{
	ReceiveResumeHUD();
}

void AEchoHUDBase::InternalHUDInit(AEchoPlayerControllerBase* InEchoPlayerController, UMenuEventHolder* InMenuEventHolder)
{
	MenuEventHolder = InMenuEventHolder;
	EchoPlayerController = InEchoPlayerController;
	
	ReceiveInternalHUDInit(InEchoPlayerController, InMenuEventHolder);
}

void AEchoHUDBase::CreateWidgets()
{
	ReceiveCreateWidgets();
}

void AEchoHUDBase::InitializeWidgets()
{
	ReceiveInitializeWidgets();	
}
