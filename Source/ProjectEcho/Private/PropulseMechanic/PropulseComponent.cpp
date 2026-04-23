#include "PropulseMechanic/PropulseComponent.h"

#include "GameFramework/Character.h"
#include "RecordManager/EchoActor.h"


UPropulseComponent::UPropulseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetGenerateOverlapEvents(true);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECR_Overlap);
}


void UPropulseComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UPropulseComponent::Propulse()
{	
	TArray<AActor*> ListActors;
	GetOverlappingActors(ListActors);
	
	for (AActor* Actor : ListActors)
	{
		if (Actor->Implements<AActor>())
		{
			if (ACharacter* character = Cast<ACharacter>(Actor))
				character->LaunchCharacter(DirectionalForce,true,false);
			else
			{
				UPrimitiveComponent* Comp = Actor->FindComponentByClass<UPrimitiveComponent>();

				if (Comp && Comp->IsSimulatingPhysics())
				{
					Comp->AddImpulse(DirectionalForce);
				}
			}
		}
	}
}

void UPropulseComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
