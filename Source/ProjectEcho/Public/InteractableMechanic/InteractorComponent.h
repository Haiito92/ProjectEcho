// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactor.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"


UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UInteractorComponent : public USceneComponent, public IInteractor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractorComponent();

	virtual bool TryInteract_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractRadius = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractDistance = 100.0f;
};
