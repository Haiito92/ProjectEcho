// Fill out your copyright notice in the Description page of Project Settings.


#include "ReflectMechanic/ReflectComponent.h"

#include "DataAssetDeveloperSettings.h"
#include "ReflectMechanic/Reflectable.h"
#include "ReflectMechanic/ReflectMechanicSettings.h"
#include "Tools/Debug/EchoDebug.h"


// Sets default values for this component's properties
UReflectComponent::UReflectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UReflectComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const UDataAssetDeveloperSettings* DataAssetDevSettings = GetDefault<UDataAssetDeveloperSettings>();
	
	ReflectMechanicSettings = DataAssetDevSettings->ReflectMechanicSettings.LoadSynchronous();
	
	if (!IsValid(ReflectMechanicSettings))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Error, "Can't init Reflect Component: reflect settings invalid", FColor::Red, 3.0f);
		return;
	}
	
	
	ReflectCooldown = ReflectMechanicSettings->ReflectCooldown;
	ReflectTimer = 0.0f;
}

void UReflectComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bIsOn) return;
	
	ReflectTimer = FMath::Max(ReflectTimer - DeltaTime, 0.0f);
	
	if (ReflectTimer <= 0.0f)
	{
		if (TryReflect())
		{
			ReflectTimer = ReflectCooldown;
		}
	}
}

void UReflectComponent::StartReflect(const FVector& InCastStartLocation, const FVector& InCastDirection)
{
	SetCastStartLocation(InCastStartLocation);
	SetCastDirection(InCastDirection);
	StartReflect();
}

void UReflectComponent::StartReflect()
{
	bIsOn = true;
	ResetCooldownTimer();
	OnReflectStarted.Broadcast();
}

void UReflectComponent::StopReflect()
{
	bIsOn = false;
	OnReflectStopped.Broadcast();
}


void UReflectComponent::ResetCooldownTimer()
{
	ReflectTimer = 0.0f;
}

bool UReflectComponent::IsOn() const
{
	return bIsOn;
}

void UReflectComponent::SetCastStartLocation(const FVector& InCastStartLocation)
{
	CastStartLocation = InCastStartLocation;
}

void UReflectComponent::SetCastDirection(const FVector& InCastDirection)
{
	CastDirection = InCastDirection;
}

bool UReflectComponent::TryReflect()
{
	if (!IsValid(ReflectMechanicSettings))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Error, "Reflect Error: reflect settings invalid", FColor::Red, 3.0f);
		return false;
	}
	
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Error, "Reflect Error: world invalid", FColor::Red, 3.0f);
		return false;
	}
	
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bReturnPhysicalMaterial = false;
	QueryParams.bTraceComplex = true;
	
	FVector CastEndLocation = CastStartLocation + CastDirection.GetSafeNormal() * ReflectMechanicSettings->SphereTraceDistance;
	
	UEchoDebug::DrawSphere(this->GetWorld(), EEchoSystem::Reflect, CastStartLocation, ReflectMechanicSettings->SphereTraceRadius, 12, FColor::Magenta, 3.0f);
	UEchoDebug::DrawSphere(this->GetWorld(), EEchoSystem::Reflect, CastEndLocation, ReflectMechanicSettings->SphereTraceRadius, 12, FColor::Magenta, 3.0f);
	
	FHitResult HitResult;
	World->SweepSingleByChannel(
		HitResult,
		CastStartLocation,
		CastEndLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(ReflectMechanicSettings->SphereTraceRadius),
		QueryParams
		);
	
	ReceiveTryReflect();

	if (!HitResult.bBlockingHit)
	{
		//UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Log, "Didn't find any reflectable", FColor::White, 3.0f);
		return false;
	}
	
	AActor* ReflectedActor = HitResult.GetActor();
	if (!IsValid(ReflectedActor) || !ReflectedActor->Implements<UReflectable>())
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Warning, "Reflect Failed: Actor is not Reflectable", FColor::Yellow, 3.0f);
		return false;
	}
	
	if (!IReflectable::Execute_CanBeReflected(ReflectedActor))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Warning, "Reflect Failed: Reflectable can't be reflected", FColor::Yellow, 3.0f);
		return false;
	}
	
	
	UPrimitiveComponent* ReflectedComp = Cast<UPrimitiveComponent>(ReflectedActor->GetRootComponent());
	if (!IsValid(ReflectedComp))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Warning, "Reflect Failed: Actor root comp is not Primitive Comp", FColor::Yellow, 3.0f);
		return false;
	}
	
	IReflectable::Execute_PrepareReflect(ReflectedActor, GetOwner());
	
	IReflectable::Execute_Reflect(ReflectedActor, CastDirection.GetSafeNormal(), ReflectMechanicSettings->ReflectPower);
	
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Log, "Reflect successfully executed!", FColor::Green, 3.0f);

	IReflectable::Execute_FinalizeReflect(ReflectedActor);
	
	OnObjectReflected.Broadcast();
	
	return true;
}

