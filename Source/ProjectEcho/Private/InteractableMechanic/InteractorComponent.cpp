// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableMechanic/InteractorComponent.h"

#include "DataAssetDeveloperSettings.h"
#include "EchoSystem.h"
#include "GrabMechanic/GrabMechanicSettings.h"
#include "InteractableMechanic/Interactable.h"
#include "InteractableMechanic/InteractMechanicSettings.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"


// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const UDataAssetDeveloperSettings* DataAssetSettings = GetDefault<UDataAssetDeveloperSettings>())
	{
		InteractMechanicSettings = DataAssetSettings->InteractMechanicSettings.LoadSynchronous();
	}
}

bool UInteractorComponent::TryInteract_Implementation(const FVector& CastStartPos, const FVector& CastDirection)
{
	if (!IsValid(InteractMechanicSettings))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Interact, EEchoMessageType::Error,"InteractSettings not found, Can't Interact !", FColor::White, 3.f);
		return false;
	}
	
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(GetOwner());

	FVector CastEndPos = CastStartPos + CastDirection.GetSafeNormal() * InteractMechanicSettings->SphereTraceDistance;
	
	FHitResult HitResult;
	GetWorld()->SweepSingleByChannel(
		HitResult,
		CastStartPos,
		CastEndPos,
		FQuat::Identity,
		ECollisionChannel::ECC_WorldStatic,
		FCollisionShape::MakeSphere(InteractMechanicSettings->SphereTraceRadius), 
		QueryParams);

	UEchoDebug::DrawSphere(this, EEchoSystem::Interact, CastStartPos, InteractMechanicSettings->SphereTraceRadius, 12, FColor::Red, 4.0f);
	UEchoDebug::DrawLine(this, EEchoSystem::Interact, CastStartPos, CastEndPos, FColor::Red, 4.0f);
	UEchoDebug::DrawSphere(this, EEchoSystem::Interact, CastEndPos, InteractMechanicSettings->SphereTraceRadius, 12, FColor::Red, 4.0f);
	
	if (!HitResult.bBlockingHit)
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Interact, EEchoMessageType::Error,
			"Failed to interact, no interactable.", FColor::Red, 5.0f);
		
		return false;
	}
	
	AActor* HitActor = HitResult.GetActor();
	
	if(HitActor->Implements<UInteractable>())
	{
		IInteractable::Execute_Interact(HitActor);
	}
	
	return true;
}

