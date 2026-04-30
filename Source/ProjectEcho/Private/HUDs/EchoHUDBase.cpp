// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/EchoHUDBase.h"

void AEchoHUDBase::InitHUD()
{
	InternalHUDInit();
	
	CreateWidgets();
	
	InitializeWidgets();
}

void AEchoHUDBase::StartHUD()
{
	ReceiveStartHUD();
}

void AEchoHUDBase::InternalHUDInit()
{
	ReceiveInternalHUDInit();
}

void AEchoHUDBase::CreateWidgets()
{
	ReceiveCreateWidgets();
}

void AEchoHUDBase::InitializeWidgets()
{
	ReceiveInitializeWidgets();	
}
