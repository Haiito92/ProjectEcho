// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifespanComponent.generated.h"


UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API ULifespanComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULifespanComponent();

	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void ResetLifespan();
	
	UFUNCTION(BlueprintCallable)
	bool IsAging();
	
	UFUNCTION(BlueprintCallable)
	void SetAging(bool Aging);
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveExpire();
private:
	UFUNCTION()
	void Expire();
	
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExpiredSignature);
	UPROPERTY(BlueprintAssignable)
	FOnExpiredSignature OnExpired;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bAging;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartLifespan;
	
	float Lifespan;
};
