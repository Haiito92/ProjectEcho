// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivableMechanic/ActivableComponent.h"


// Sets default values for this component's properties
UActivableComponent::UActivableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	PowerForActivation = 1;
}

void UActivableComponent::AddPower_Implementation()
{
	IActivable::AddPower_Implementation();
	
	PowerAmount += 1;
	
	if (PowerAmount >= PowerForActivation)
	{
		Execute_Activate(this);
	}
}

void UActivableComponent::RemovePower_Implementation()
{
	IActivable::RemovePower_Implementation();
	
	PowerAmount = FMath::Max(PowerAmount - 1, 0);
	
	if (PowerAmount == PowerForActivation - 1)
	{
		Execute_Deactivate(this);
	}
}

void UActivableComponent::Activate_Implementation()
{
	OnActivated.Broadcast();
}

void UActivableComponent::Deactivate_Implementation()
{
	OnDeactivated.Broadcast();
}

bool UActivableComponent::IsActivated_Implementation() const
{
	return bActivated;
}


