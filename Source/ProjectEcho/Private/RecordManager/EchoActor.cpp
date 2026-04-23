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

void AEchoActor::HandleActionKey(TSharedPtr<FRecordedAction> Action)
{
	//Switch on Enum for Only Cpp Actions
	switch (Action->ActionEnum)
	{
	case ERecordedAction::ForceGrab:
		{
			TSharedPtr<FRecordedForceGrabAction> ForceGrabAction = StaticCastSharedPtr<FRecordedForceGrabAction>(Action);
			if (ForceGrabAction != nullptr)
			{
				FindComponentByClass<UGrabbingComponent>()->ForceGrab(ForceGrabAction->ActorToGrab);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Cant Get ForceGrab !!!");
			}
			break;
		}
	default: 
		break;
	}
	
	//Call BP Function
	ReceiveHandleActionKey(Action->ActionEnum);
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

