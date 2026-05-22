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

bool UGrabbingComponent::TryGrab(const FRotator& ControlRotation, const FGrabbingRules& GrabbingRules)
{
	if (IsGrabbing()) return false;
	if (!IsValid(GrabMechanicSettings))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Grab, EEchoMessageType::Error,"GrabSettings not found, Can't Grab !", FColor::White, 3.f);
		return false;
	}
	
	
	FHitResult HitResult;
	
	FVector CastStartLocation = GetOwner()->GetActorLocation() + GrabMechanicSettings->SphereTraceBaseLocationOffset + (ControlRotation.RotateVector(FVector(1.f, 0.f, 0.f)));
	FVector CastEndLocation = CastStartLocation + ControlRotation.RotateVector(FVector(GrabMechanicSettings->SphereTraceDistance, 0.f, 0.f));
	
	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
	TraceParams.bTraceComplex = true;
	//TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(GetOwner());

	GetWorld()->SweepSingleByChannel(
		HitResult,
		CastStartLocation,
		CastEndLocation,
		ControlRotation.Quaternion(),
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(GrabMechanicSettings->SphereTraceRadius), 
		TraceParams);
	
	UEchoDebug::DrawSphere(GetWorld(), EEchoSystem::Grab, CastStartLocation, GrabMechanicSettings->SphereTraceRadius, 12, FColor::White, 3.f);
	UEchoDebug::DrawSphere(GetWorld(), EEchoSystem::Grab, CastEndLocation, GrabMechanicSettings->SphereTraceRadius, 12, FColor::White, 3.f);
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Grab, EEchoMessageType::Log, (HitResult.bBlockingHit ? "Try Grabbing Actor : " + HitResult.GetActor()->GetName() : "Failed to Grab at current Location"), FColor::White, 3.f);
	
	if (HitResult.bBlockingHit)
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UGrabbableInterface::StaticClass()) && IGrabbableInterface::Execute_CanBeGrabbed(HitResult.GetActor()))
		{
			GrabbedActor = HitResult.GetActor();
			IGrabbableInterface::Execute_OnBeforeGrabbed(GrabbedActor, this->GetOwner(), GrabbingRules);
			OnWillGrabActor.Broadcast(GrabbedActor);
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
			GrabbedActor->AttachToComponent(this, AttachmentTransformRules);
			IGrabbableInterface::Execute_OnGrabbed(GrabbedActor, this->GetOwner());
			return true;
		}
	}
	return false;
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

void UGrabbingComponent::ForceRelease()
{
	if (IsGrabbing())
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Grab, EEchoMessageType::Log,"Force Releasing Grabbed Actor : " + GrabbedActor->GetName(), FColor::White, 3.f);
		GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		IGrabbableInterface::Execute_OnObjectForceReleased(GrabbedActor);
		GrabbedActor = nullptr;
	}
}

void UGrabbingComponent::ForceGrab(AActor* Actor, const FGrabbingRules& GrabbingRules)
{
	if (!IsGrabbing())
	{
		if (Actor->GetClass()->ImplementsInterface(UGrabbableInterface::StaticClass()) && IGrabbableInterface::Execute_CanBeGrabbed(Actor))
		{
			GrabbedActor = Actor;
			IGrabbableInterface::Execute_OnObjectBeforeForceGrabbed(GrabbedActor, GrabbingRules);
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
			GrabbedActor->AttachToComponent(this, AttachmentTransformRules);
			IGrabbableInterface::Execute_OnObjectForceGrabbed(GrabbedActor, this->GetOwner());
		}
	}
}

void UGrabbingComponent::TryForceGrabHeldCube(const FGrabbingRules& GrabbingRules)
{
	if (IsGrabbing()) return;
	if (!IsValid(GrabMechanicSettings))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Grab, EEchoMessageType::Error,"GrabSettings not found, Can't Grab !", FColor::White, 3.f);
		return;
	}
	FHitResult HitResult;
	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
	TraceParams.bTraceComplex = true;
	//TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(GetOwner());

	GetWorld()->SweepSingleByChannel(
		HitResult,
		GetComponentLocation(),
		GetComponentLocation(),
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(GrabMechanicSettings->SphereTraceRadius),
		TraceParams);
	
	UEchoDebug::DrawSphere(GetWorld(), EEchoSystem::Grab, GetComponentLocation(), GrabMechanicSettings->SphereTraceRadius, 12, FColor::White, 3.f);
	
	if (HitResult.bBlockingHit)
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UGrabbableInterface::StaticClass()) && IGrabbableInterface::Execute_CanBeGrabbed(HitResult.GetActor()))
		{
			GrabbedActor = HitResult.GetActor();
			IGrabbableInterface::Execute_OnObjectBeforeForceGrabbed(GrabbedActor, GrabbingRules);
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
			GrabbedActor->AttachToComponent(this, AttachmentTransformRules);
			IGrabbableInterface::Execute_OnObjectForceGrabbed(GrabbedActor, this->GetOwner());
		}
	}
	else
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Grab, EEchoMessageType::Error,"Failed to Try ForceGrab Held Cube (Nothing found on SphereCast)", FColor::White, 3.f);
	}
}

bool UGrabbingComponent::IsGrabbing()
{
	return IsValid(GrabbedActor);
}

AActor* UGrabbingComponent::GetGrabbedActor()
{
	return GrabbedActor;
}

