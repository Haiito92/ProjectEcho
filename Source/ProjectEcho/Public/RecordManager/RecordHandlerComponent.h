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

	UFUNCTION(BlueprintCallable)
	virtual void RegisterActionInRecord(const FRecordedAction& Action, const FRecordedAction& RewindAction = FRecordedAction());
	
	UFUNCTION(BlueprintCallable)
	virtual void RegisterFloatAnimationKey(const EAnimationValueReference& AnimationValueRef, const float& Value);
	
	UFUNCTION(BlueprintCallable)
	virtual void RegisterBoolAnimationKey(const EAnimationValueReference& AnimationValueRef, const bool& Value);
	
	UFUNCTION(BlueprintCallable)
	//Start Saving Actions in ToRecord List
	virtual void StartRecording();
	
	UFUNCTION(BlueprintCallable)
	//Stop Saving Actions in ToRecord List and Reset List
	virtual void StopRecording();
	
	//Returns Copy of Current ToRecordList and Reset it
	UFUNCTION()
	virtual TArray<FRecordedAction> GetToRecordActions();
	
	//Returns Copy of Current ToRecordList of Rewind Actions and Reset it
	UFUNCTION()
	virtual TArray<FRecordedAction> GetToRecordRewindActions();
	
	//Returns Copy of Current ToRecordList of AnimationKeys and Reset it 
	UFUNCTION()
	virtual TArray<FRecordAnimationValue> GetToRecordAnimationKeys();

private:
	//Array of Actions to register in next Key Creation
	UPROPERTY()
	TArray<FRecordedAction> ToRecordActions;
	
	//Array of Actions to register in next Key Creation
	UPROPERTY()
	TArray<FRecordedAction> ToRecordRewindActions;
	
	//Array of AnimationKeys to register in next Key Creation
	UPROPERTY()
	TArray<FRecordAnimationValue> ToRecordAnimationKeys;
	
	bool bIsRecording = false;
};
