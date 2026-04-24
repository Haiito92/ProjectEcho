#include "PropulseMechanic/PropulseComponent.h"

#include "DataAssetDeveloperSettings.h"
#include "GameFramework/Character.h"
#include "PropulseMechanic/Propulsable.h"
#include "PropulseMechanic/PropulseMechanicSettings.h"
#include "Tools/Debug/EchoDebug.h"


UPropulseComponent::UPropulseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetGenerateOverlapEvents(true);
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Overlap);
}


void UPropulseComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const UDataAssetDeveloperSettings* DataAssetDevSettings = GetDefault<UDataAssetDeveloperSettings>();
	
	PropulseMechanicSettings = DataAssetDevSettings->PropulseMechanicSettings.LoadSynchronous();
}

bool UPropulseComponent::TryPropulse()
{	
	if (!IsValid(PropulseMechanicSettings))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Propulse, EEchoMessageType::Error, "Propulse Error: propulse settings invalid", FColor::Red, 3.0f);
		return false;
	}
	
	void ReceiveTryPropulse();
	
	TArray<AActor*> ListActors;
	GetOverlappingActors(ListActors);
	
	UEchoDebug::DrawSphere(this->GetWorld(), EEchoSystem::Propulse, GetComponentLocation(), SphereRadius, 12, FColor::Magenta, 3.0f);
	
	bool DidPropulseActors = false;
	for (AActor* PropulsedActor : ListActors)
	{
		if (PropulsedActor == GetOwner() || !IsValid(PropulsedActor)) continue;
		
		if (!PropulsedActor->Implements<UPropulsable>())
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Propulse, EEchoMessageType::Warning, "Propulse Failed: Actor is not Propulsable", FColor::Yellow, 3.0f);
			continue;
		}
		
		if (!IPropulsable::Execute_CanBePropulsed(PropulsedActor))
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Propulse, EEchoMessageType::Warning, "Propulse Failed: Propulsable can't be propulsed", FColor::Yellow, 3.0f);
			continue;
		}

		UPrimitiveComponent* PropulsedComp = Cast<UPrimitiveComponent>(PropulsedActor->GetRootComponent());
		if (!IsValid(PropulsedComp))
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Propulse, EEchoMessageType::Warning, "Propulse Failed: Actor root comp is not Primitive Comp", FColor::Yellow, 3.0f);
			continue;
		}
		
		IPropulsable::Execute_PreparePropulse(PropulsedActor, GetOwner());
		
		IPropulsable::Execute_Propulse(PropulsedActor, FVector::UpVector, PropulseMechanicSettings->PropulsePower);
		
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Propulse, EEchoMessageType::Log, "Propulse successfully executed!", FColor::Green, 3.0f);
		
		IPropulsable::Execute_FinalizePropulse(PropulsedActor);
		
		DidPropulseActors = true;
	}
	
	return DidPropulseActors;
}
