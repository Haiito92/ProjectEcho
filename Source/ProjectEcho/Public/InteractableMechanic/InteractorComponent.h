// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactor.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "InteractorComponent.generated.h"


UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UInteractorComponent : public USphereComponent, public IInteractor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractorComponent();

	virtual bool TryInteract_Implementation() override;
};
