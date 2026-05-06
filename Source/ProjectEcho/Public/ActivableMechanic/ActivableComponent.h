// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Activable.h"
#include "Components/ActorComponent.h"
#include "ActivableComponent.generated.h"


UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API UActivableComponent : public UActorComponent, public IActivable
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActivableComponent();

	virtual void AddPower_Implementation() override;
	virtual void RemovePower_Implementation() override;
	
	virtual void Activate_Implementation() override;
	virtual void Deactivate_Implementation() override;
	
	virtual bool IsActivated_Implementation() const override;
	
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivatedSignature);
	UPROPERTY(BlueprintAssignable)
	FOnActivatedSignature OnActivated;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeactivatedSignature);
	UPROPERTY(BlueprintAssignable)
	FOnDeactivatedSignature OnDeactivated;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bActivated;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int PowerAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=1))
	int PowerForActivation;
};
