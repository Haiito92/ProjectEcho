// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values
#include <RecordManager/EchoActor.h>

#include "GrabMechanic/GrabbingComponent.h"
#include "RecordManager/RecordableInterface.h"
#include "RecordManager/RecordManagerSubsystem.h"

AEchoActor::AEchoActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEchoActor::BeginPlay()
{
	Super::BeginPlay();
	GrabbingComponent = GetComponentByClass<UGrabbingComponent>();
}

void AEchoActor::HandleActionKey(const FRecordedAction& Action)
{
	//Call BP Function
	ReceiveHandleActionKey(Action.ActionEnum);
}

void AEchoActor::ForceRelease_Implementation()
{
	if (IsValid(GrabbingComponent)) GrabbingComponent->ForceRelease();
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

void AEchoActor::HandleRewindStarted(const float& CurrentTimeKey)
{
	ReceiveHandleRewindStarted(CurrentTimeKey);
}

void AEchoActor::HandleRewindStopped(const float& CurrentTimeKey)
{
	ReceiveHandleRewindStopped(CurrentTimeKey);
}

void AEchoActor::OnTimelineDestroyed()
{
	//Call BP Function
	ReceiveOnTimelineDestroyed();
}

void AEchoActor::Laserize_Implementation()
{
	ILaserizable::Laserize_Implementation();
	
	OnEchoDestroyed.Broadcast(EchoIndex);
}

void AEchoActor::RegisterRecordable(TScriptInterface<IRecordableInterface> Recordable)
{
	OnRegisterRecordable.Broadcast(Recordable);
}

