// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableMechanic/InteractorComponent.h"

#include "EchoSystem.h"
#include "InteractableMechanic/Interactable.h"
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

bool UInteractorComponent::TryInteract_Implementation(const FVector& CastStartLocation, const FVector& CastForwardVector)
{
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(GetOwner());

	FVector CastEndLocation = CastStartLocation + CastForwardVector * InteractDistance;
	
	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByChannel(
		HitResults,
		CastStartLocation,
		CastEndLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_WorldStatic,
		FCollisionShape::MakeSphere(InteractRadius),
		QueryParams);

	UEchoDebug::DrawSphere(this, EEchoSystem::Interact, CastStartLocation, InteractRadius, 12, FColor::Red, 4.0f);
	UEchoDebug::DrawLine(this, EEchoSystem::Interact, CastStartLocation, CastEndLocation, FColor::Red, 4.0f);
	UEchoDebug::DrawSphere(this, EEchoSystem::Interact, CastEndLocation, InteractRadius, 12, FColor::Red, 4.0f);
	
	if (HitResults.IsEmpty())
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Interact, EEchoMessageType::Error,
			"Failed to interact, no interactable.", FColor::Red, 5.0f);
		
		return false;
	}
	
	for (FHitResult HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		
		if(HitActor->Implements<UInteractable>())
		{
			IInteractable::Execute_Interact(HitActor);
		}
	}
	
	return false;
}

