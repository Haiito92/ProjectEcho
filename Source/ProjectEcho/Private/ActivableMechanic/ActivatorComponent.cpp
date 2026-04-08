// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivableMechanic/ActivatorComponent.h"

#include "ActivableMechanic/Activable.h"


// Sets default values for this component's properties
UActivatorComponent::UActivatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UActivatorComponent::ToggleSwitch_Implementation()
{
	if (bIsSwitchOn)
	{
		Execute_SwitchOff(this);
	}
	else
	{
		Execute_SwitchOn(this);
	}
}

void UActivatorComponent::SwitchOn_Implementation()
{
	bIsSwitchOn = true;

	for (AActor* Activable : Activables)
	{
		if (Activable->Implements<UActivable>())
		{
			IActivable::Execute_Activate(Activable);
		}
	} 
}

void UActivatorComponent::SwitchOff_Implementation()
{
	bIsSwitchOn = false;

	for (AActor* Activable : Activables)
	{
		if (Activable->Implements<UActivable>())
		{
			IActivable::Execute_Deactivate(Activable);
		}
	}
}

bool UActivatorComponent::IsSwitchOn_Implementation()
{
	return bIsSwitchOn;
}

