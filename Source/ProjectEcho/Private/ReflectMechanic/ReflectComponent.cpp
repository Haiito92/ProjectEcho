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
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UReflectComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const UDataAssetDeveloperSettings* DataAssetDevSettings = GetDefault<UDataAssetDeveloperSettings>();
	
	ReflectMechanicSettings = DataAssetDevSettings->ReflectMechanicSettings.LoadSynchronous();
}

bool UReflectComponent::TryReflect(const FVector& CastStartLocation, const FVector& CastDirection)
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
	
	UEchoDebug::DrawSphere(this->GetWorld(), EEchoSystem::Reflect, CastEndLocation, ReflectMechanicSettings->SphereTraceRadius, 12, FColor::Magenta, 3.0f);
	
	FHitResult HitResult;
	World->SweepSingleByChannel(
		HitResult,
		CastEndLocation,
		CastEndLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_WorldDynamic,
		FCollisionShape::MakeSphere(ReflectMechanicSettings->SphereTraceRadius),
		QueryParams
		);

	if (HitResult.bBlockingHit)
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Log, "Didn't find any reflectable", FColor::White, 3.0f);
		return false;
	}
	
	AActor* ReflectedActor = HitResult.GetActor();
	if (!IsValid(ReflectedActor) || !ReflectedActor->Implements<UReflectable>())
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Warning, "Reflect Failed: Actor is not Reflectable", FColor::Yellow, 3.0f);
		return false;
	}
	
	UPrimitiveComponent* ReflectedComp = Cast<UPrimitiveComponent>(ReflectedActor->GetRootComponent());
	if (!IsValid(ReflectedComp))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Reflect, EEchoMessageType::Warning, "Reflect Failed: Actor root comp is not Primitive Comp", FColor::Yellow, 3.0f);
		return false;
	}
	
	FVector ReflectForce = CastDirection.GetSafeNormal() * ReflectMechanicSettings->ReflectPower;
	ReflectedComp->AddImpulse(ReflectForce);
	
	return true;
}

