// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RecordHandlerComponent.generated.h"


enum class ERecordedAction : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API URecordHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URecordHandlerComponent();

	UFUNCTION(BlueprintCallable)
	virtual void RegisterActionInRecord(ERecordedAction Action);
	
	UFUNCTION(BlueprintCallable)
	//Start Saving Actions in ToRecord List
	virtual void StartRecording();
	
	UFUNCTION(BlueprintCallable)
	//Stop Saving Actions in ToRecord List and Reset List
	virtual void StopRecording();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	//Returns Copy of Current ToRecordList and Reset it
	virtual TArray<ERecordedAction> GetToRecordActions();
	
	
private:
	//Array of Actions to register in next Key Creation
	TArray<ERecordedAction> ToRecordActions;
	
	bool bIsRecording = false;
};
