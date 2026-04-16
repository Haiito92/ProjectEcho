// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values
#include <RecordManager/EchoActor.h>

#include "RecordManager/RecordableInterface.h"

AEchoActor::AEchoActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEchoActor::HandleActionKey(ERecordedAction Action)
{
	//Call BP Function
	ReceiveHandleActionKey(Action);
}

void AEchoActor::SetControlRotation(const FRotator& ControlRotation)
{
	//Call BP Function
	ReceiveSetControlRotation(ControlRotation);
}

void AEchoActor::InitEcho(const int& index, const FEchoColorStruct& EchoColor)
{
	EchoIndex = index;
	ReceiveInitEcho(EchoColor);
}

void AEchoActor::RegisterRecordable(TScriptInterface<IRecordableInterface> Recordable)
{
	OnRegisterRecordable.Broadcast(Recordable);
}

