// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecordKeysStructs.h"

#include "RecordHandlerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTECHO_API URecordHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URecordHandlerComponent();

	virtual void RegisterActionInRecord(TSharedPtr<FRecordedAction> Action, TSharedPtr<FRecordedAction> RewindAction = nullptr);
	
	UFUNCTION(BlueprintCallable)
	//Start Saving Actions in ToRecord List
	virtual void StartRecording();
	
	UFUNCTION(BlueprintCallable)
	//Stop Saving Actions in ToRecord List and Reset List
	virtual void StopRecording();
	
	//Returns Copy of Current ToRecordList and Reset it
	virtual TArray<TSharedPtr<FRecordedAction>> GetToRecordActions();
	
	//Returns Copy of Current ToRecordList of Rewind Actions and Reset it
	virtual TArray<TSharedPtr<FRecordedAction>> GetToRecordRewindActions();

private:
	//Array of Actions to register in next Key Creation
	TArray<TSharedPtr<FRecordedAction>> ToRecordActions;
	
	//Array of Actions to register in next Key Creation
	TArray<TSharedPtr<FRecordedAction>> ToRecordRewindActions;
	
	bool bIsRecording = false;
};
