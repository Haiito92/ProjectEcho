// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactor.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "InteractorComponent.generated.h"


class UInteractMechanicSettings;

UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UInteractorComponent : public UActorComponent, public IInteractor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool TryInteract_Implementation(const FVector& CastStartPos, const FVector& CastDirection) override;

protected:
	UPROPERTY()
	TObjectPtr<UInteractMechanicSettings> InteractMechanicSettings;
};
