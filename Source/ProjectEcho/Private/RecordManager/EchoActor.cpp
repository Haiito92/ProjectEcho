// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values
#include <RecordManager/EchoActor.h>

#include "GrabMechanic/GrabbingComponent.h"
#include "RecordManager/RecordableInterface.h"
#include "RecordManager/RecordManagerSubsystem.h"
#include "ReflectMechanic/ReflectComponent.h"
#include "WorldPartition/HLOD/DestructibleHLODComponent.h"

AEchoActor::AEchoActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEchoActor::BeginPlay()
{
	Super::BeginPlay();
	GrabbingComponent = FindComponentByClass<UGrabbingComponent>();
	ReflectComponent = FindComponentByClass<UReflectComponent>();
}

void AEchoActor::HandleActionKey(const FRecordedAction& Action)
{
	//Call BP Function
	ReceiveHandleActionKey(Action.ActionEnum);
}

void AEchoActor::HandleAnimationKey(const FRecordAnimationValue& AnimationValue)
{
	//Call BP Function
	switch (AnimationValue.AnimationValueType)
	{
	case EAnimationValueType::Bool:
		ReceiveHandleAnimationBooleanKey(AnimationValue.AnimationValueReference, AnimationValue.BoolValue);
		break;
	case EAnimationValueType::Float:
		ReceiveHandleAnimationFloatKey(AnimationValue.AnimationValueReference, AnimationValue.FloatValue);
		break;
	default:
		break;
	}
}

void AEchoActor::ForceRelease_Implementation()
{
	if (IsValid(GrabbingComponent)) GrabbingComponent->ForceRelease();
}

USkeletalMeshComponent* AEchoActor::GetSkeletalMeshComponent_Implementation()
{
	return ReceiveGetSkeletalMeshComponent();
}

void AEchoActor::TakeStateSnapshot()
{
	if (IsValid(ReflectComponent)) EchoStateSnapshot.bIsReflecting = ReflectComponent->IsOn();
}

void AEchoActor::RestoreStateSnapshot()
{
	if (IsValid(ReflectComponent))
	{
		if (ReflectComponent->IsOn() != EchoStateSnapshot.bIsReflecting)
		{
			if (ReflectComponent->IsOn()) ReflectComponent->StopReflect();
			else ReflectComponent->StartReflect();
		}
	}
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

void AEchoActor::HandleRewindStarted(const float& CurrentTimeKey, bool bIsPlayerRewind)
{
	ReceiveHandleRewindStarted(CurrentTimeKey);
}

void AEchoActor::HandleRewindStopped(const float& CurrentTimeKey, bool bIsPlayerRewind)
{
	if (bIsPlayerRewind)
	{
		RestoreStateSnapshot();
	}
	
	ReceiveHandleRewindStopped(CurrentTimeKey);
}

void AEchoActor::HandleRecordStarted(const float& CurrentTimeKey)
{
	TakeStateSnapshot();
}

void AEchoActor::HandleTimelineActivation(bool bIsActive)
{
	//Call BP Function
	ReceiveHandleTimelineActivation(bIsActive);
}

void AEchoActor::OnTimelineDestroyed()
{
	//Call BP Function
	ReceiveOnTimelineDestroyed();
}

void AEchoActor::Laserize_Implementation()
{
	ILaserizable::Laserize_Implementation();
	
	Execute_ReceiveLaserize(this);
	
	OnEchoDestroyed.Broadcast(EchoIndex);
}

void AEchoActor::HandleFootstep_Implementation(bool bRightFoot)
{
	//Call BP Function
	ReceiveHandleFootstep(bRightFoot);
}

void AEchoActor::RegisterRecordable(TScriptInterface<IRecordableInterface> Recordable)
{
	OnRegisterRecordable.Broadcast(Recordable);
}

FRotator AEchoActor::GetEchoControlRotation_Implementation()
{
	return FRotator::ZeroRotator;
}

