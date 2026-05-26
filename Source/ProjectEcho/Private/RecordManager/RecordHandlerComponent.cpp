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

void URecordHandlerComponent::RegisterActionInRecord(const FRecordedAction& Action, const FRecordedAction& RewindAction)
{
	if (bIsRecording)
	{
		ToRecordActions.Add(Action);
		if (RewindAction.ActionEnum != ERecordedAction::None) ToRecordRewindActions.Add(RewindAction);
	}
}

void URecordHandlerComponent::RegisterFloatAnimationKey(const EAnimationValueReference& AnimationValueRef,
	const float& Value)
{
	if (bIsRecording)
	{
		ToRecordAnimationKeys.Add(FRecordAnimationFloatKey(AnimationValueRef, Value));
	}
}

void URecordHandlerComponent::RegisterBoolAnimationKey(const EAnimationValueReference& AnimationValueRef,
	const bool& Value)
{
	if (bIsRecording)
	{
		ToRecordAnimationKeys.Add(FRecordAnimationBooleanKey(AnimationValueRef, Value));
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

TArray<FRecordedAction> URecordHandlerComponent::GetToRecordActions()
{
	TArray<FRecordedAction> CopyList = ToRecordActions;
	ToRecordActions.Empty();
	return CopyList;
}

TArray<FRecordedAction> URecordHandlerComponent::GetToRecordRewindActions()
{
	TArray<FRecordedAction> CopyList = ToRecordRewindActions;
	ToRecordRewindActions.Empty();
	return CopyList;
}

TArray<FRecordAnimationKey> URecordHandlerComponent::GetToRecordAnimationKeys()
{
	TArray<FRecordAnimationKey> CopyList = ToRecordAnimationKeys;
	ToRecordAnimationKeys.Empty();
	return CopyList;
}

