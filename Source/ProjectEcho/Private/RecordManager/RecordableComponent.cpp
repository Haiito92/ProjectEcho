// Fill out your copyright notice in the Description page of Project Settings.

#include "RecordManager/RecordableComponent.h"

// Sets default values for this component's properties
URecordableComponent::URecordableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void URecordableComponent::RegisterActionInRecord(ERecordedAction Action)
{
	if (bIsRecording) ToRecordActions.Add(Action);
}

void URecordableComponent::StartRecording()
{
	bIsRecording = true;
}

void URecordableComponent::StopRecording()
{
	bIsRecording = false;
}

TArray<ERecordedAction> URecordableComponent::GetToRecordActions()
{
	TArray<ERecordedAction> CopyList = ToRecordActions;
	ToRecordActions.Empty();
	return CopyList;
}

