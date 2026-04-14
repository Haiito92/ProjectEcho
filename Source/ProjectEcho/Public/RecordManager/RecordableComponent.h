// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecordKeysStructs.h"

#include "RecordableComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API URecordableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URecordableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	//Record a TransformKey at CurrentTimeKey (given in global Timeline) for Owner
	UFUNCTION()
	void RecordKey(const float& CurrentTimeKey);
	
	UFUNCTION()
	//Replay Current Transform Key (given in global Timeline) in Rewind for Owner
	void ReplayKey(const float& PreviousTimeKey, const float& CurrentTimeKey);
	
	UFUNCTION()
	void StartRewind();
private:
	UPROPERTY()
	TArray<FRecordTransformKey> TransformKeys;
	
	const FRecordTransformKey* FindClosestTransformKey(const float& CurrentTimeKey, bool bFindNextOne);
};
