// Fill out your copyright notice in the Description page of Project Settings.

#include "RecordManager/RecordHandlerComponent.h"

// Sets default values for this component's properties
URecordHandlerComponent::URecordHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void URecordHandlerComponent::RegisterActionInRecord(TSharedPtr<FRecordedAction> Action, TSharedPtr<FRecordedAction> RewindAction)
{
	if (bIsRecording && Action != nullptr)
	{
		ToRecordActions.Add(Action);
		if (RewindAction != nullptr) ToRecordRewindActions.Add(RewindAction);
	}
}

void URecordHandlerComponent::StartRecording()
{
	bIsRecording = true;
}

void URecordHandlerComponent::StopRecording()
{
	bIsRecording = false;
}

TArray<TSharedPtr<FRecordedAction>> URecordHandlerComponent::GetToRecordActions()
{
	TArray<TSharedPtr<FRecordedAction>> CopyList = ToRecordActions;
	ToRecordActions.Empty();
	return CopyList;
}

TArray<TSharedPtr<FRecordedAction>> URecordHandlerComponent::GetToRecordRewindActions()
{
	TArray<TSharedPtr<FRecordedAction>> CopyList = ToRecordRewindActions;
	ToRecordActions.Empty();
	return CopyList;
}

