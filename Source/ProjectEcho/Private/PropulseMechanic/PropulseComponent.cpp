#include "PropulseMechanic/PropulseComponent.h"

#include "GameFramework/Character.h"
#include "RecordManager/EchoActor.h"


UPropulseComponent::UPropulseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPropulseComponent::BeginPlay()
{
	Super::BeginPlay();	
}

bool UPropulseComponent::TryPropulse()
{
	if (ACharacter* character = Cast<ACharacter>(this->GetOwner()))
	{
		character->LaunchCharacter(DirectionalForce,false,false);
	}
	else if (AEchoActor* echoActor = Cast<AEchoActor>(this->GetOwner()))
	{
		//echoActor->Get;
	}
}


void UPropulseComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
