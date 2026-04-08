// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Activator.h"
#include "Components/ActorComponent.h"
#include "ActivatorComponent.generated.h"


class UActivable;
class IActivable;

UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UActivatorComponent : public UActorComponent, public IActivator
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActivatorComponent();

	virtual void ToggleSwitch_Implementation() override;
	virtual void SwitchOn_Implementation() override;
	virtual void SwitchOff_Implementation() override;

	virtual bool IsSwitchOn_Implementation() override;
protected:
	UPROPERTY(EditAnywhere)
	TArray<AActor*> Activables;

private:
	bool bIsSwitchOn;
};
