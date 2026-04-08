// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values for this component's properties
#include <GrabMechanic/GrabbingComponent.h>

#include "DataAssetDeveloperSettings.h"
#include "GrabMechanic/GrabbableInterface.h"
#include "GrabMechanic/GrabMechanicSettings.h"
#include "Tools/Debug/EchoDebug.h"

UGrabbingComponent::UGrabbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGrabbingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GrabMechanicSettings = GetDefault<UDataAssetDeveloperSettings>()->GrabMechanicSettings.LoadSynchronous();
}

bool UGrabbingComponent::TryGrab(const FRotator& ControlRotation)
{
	if (IsGrabbing()) return false;
	if (!IsValid(GrabMechanicSettings))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Grab, EEchoMessageType::Error,"GrabSettings not found, Can't Grab !", FColor::White, 3.f);
		return false;
	}
	FHitResult HitResult;
	FVector CastLocation = GetOwner()->GetActorLocation() + GrabMechanicSettings->SphereTraceBaseLocationOffset + (ControlRotation.RotateVector(FVector(GrabMechanicSettings->SphereTraceDistance, 0.f, 0.f)));
	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
	TraceParams.bTraceComplex = true;
	//TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(GetOwner());

	GetWorld()->SweepSingleByChannel(
		HitResult,
		CastLocation,
		CastLocation,
		ControlRotation.Quaternion(),
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(GrabMechanicSettings->SphereTraceRadius), 
		TraceParams);
	
	UEchoDebug::DrawSphere(GetWorld(), EEchoSystem::Grab, CastLocation, GrabMechanicSettings->SphereTraceRadius, 12, FColor::White, 3.f);
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Grab, EEchoMessageType::Log, (HitResult.bBlockingHit ? "Try Grabbing Actor : " + HitResult.GetActor()->GetName() : "Failed to Grab at current Location"), FColor::White, 3.f);
	
	if (HitResult.bBlockingHit)
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UGrabbableInterface::StaticClass()) && IGrabbableInterface::Execute_CanBeGrabbed(HitResult.GetActor()))
		{
			GrabbedActor = HitResult.GetActor();
			IGrabbableInterface::Execute_OnBeforeGrabbed(GrabbedActor);
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
			GrabbedActor->AttachToComponent(this, AttachmentTransformRules);
			IGrabbableInterface::Execute_OnGrabbed(GrabbedActor);
		}
	}
	return true;
}

bool UGrabbingComponent::TryRelease()
{
	if (IsGrabbing())
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Grab, EEchoMessageType::Log,"Releasing Grabbed Actor : " + GrabbedActor->GetName(), FColor::White, 3.f);
		GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		IGrabbableInterface::Execute_OnObjectReleased(GrabbedActor);
		GrabbedActor = nullptr;
		return true;
	}
	return false;
}

bool UGrabbingComponent::TryThrow(const FRotator& ControlRotation)
{
	if (IsGrabbing())
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Grab, EEchoMessageType::Log,"Throwing Grabbed Actor : " + GrabbedActor->GetName(), FColor::White, 3.f);
		GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		IGrabbableInterface::Execute_OnThrown(GrabbedActor, ControlRotation.Vector(), GrabMechanicSettings->ThrowStrength);
		GrabbedActor = nullptr;
		return true;
	}
	return false;
}

bool UGrabbingComponent::IsGrabbing()
{
	return IsValid(GrabbedActor);
}

AActor* UGrabbingComponent::GetGrabbedActor()
{
	return GrabbedActor;
}

