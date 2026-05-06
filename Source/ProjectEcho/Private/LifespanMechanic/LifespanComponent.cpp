// Fill out your copyright notice in the Description page of Project Settings.


#include "LifespanMechanic/LifespanComponent.h"


// Sets default values for this component's properties
ULifespanComponent::ULifespanComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
	StartLifespan = 2.0f;
}

void ULifespanComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Lifespan = StartLifespan;
}


// Called every frame
void ULifespanComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bAging && Lifespan > 0)
	{
		Lifespan -= DeltaTime;
		
		if (Lifespan <= 0)
		{
			Expire();
		}
	}
}

void ULifespanComponent::ResetLifespan()
{
	Lifespan = StartLifespan;
}

bool ULifespanComponent::IsAging()
{
	return bAging;
}

void ULifespanComponent::SetAging(bool Aging)
{
	bAging = Aging;
}

void ULifespanComponent::Expire()
{
	OnExpired.Broadcast();
	ReceiveExpire();
}

