#include <RecordManager/RecordManagerSubsystem.h>

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#pragma region Timeline

const FRecordTransformKey* FEchoTimeline::GetNextTransformKey(const float& TimeKey) const
{
	for (const FRecordTransformKey& TransformKey : TransformKeys)
	{
		if (TransformKey.TimeKey >= TimeKey)
		{
			return &TransformKey;
		}
	}
	return nullptr;
}

const FRecordTransformKey* FEchoTimeline::GetPreviousTransformKey(const float& TimeKey) const
{
	const FRecordTransformKey* key = nullptr;
	for (const FRecordTransformKey& TransformKey : TransformKeys)
	{
		if (TransformKey.TimeKey <= TimeKey)
		{
			key = &TransformKey;
		}
		else
		{
			break;
		}
	}
	return key;
}

const float& FEchoTimeline::GetLastTimeKey()
{
	return TransformKeys[TransformKeys.Num() - 1].TimeKey;
}

void FEchoTimeline::RegisterEchoActor(AActor* InEchoActor)
{
	EchoActor = InEchoActor;
}

void FEchoTimeline::RecordTransformKey(AActor* RecordedActor, const float& CurrentTimeKey)
{
	//Create and Init values of TransformKey
	FRecordTransformKey TransformKey;
	TransformKey.TimeKey = CurrentTimeKey;
	TransformKey.Position = RecordedActor->GetActorLocation();
	TransformKey.Rotation = RecordedActor->GetActorRotation();
	TransformKey.Scale = RecordedActor->GetActorScale();
	
	//Add it to the list and Sort the list (list has to be in order)
	TransformKeys.Add(TransformKey);
	TransformKeys.Sort([](const FRecordTransformKey& A, const FRecordTransformKey& B)
	{
		return A.TimeKey < B.TimeKey;
	});
}

void FEchoTimeline::PlayReplay(const float& PreviousKey,const float& CurrentTimeKey, bool bIsInRewind)
{
	if (!IsValid(EchoActor)) return;
	if (GetLastTimeKey() < CurrentTimeKey) return;
	
	//Play Transform Key (Lerp between two closest Keys)
	const FRecordTransformKey* PreviousTransformKey = GetPreviousTransformKey(CurrentTimeKey);
	const FRecordTransformKey* NextTransformKey = GetNextTransformKey(CurrentTimeKey);
	
	//Place Actor according to previous and next TransformKey 
	float lerpValue = (CurrentTimeKey - PreviousTransformKey->TimeKey) / (NextTransformKey->TimeKey - PreviousTransformKey->TimeKey);
	EchoActor->SetActorLocation(FMath::Lerp(PreviousTransformKey->Position, NextTransformKey->Position, lerpValue));
	EchoActor->SetActorRotation(FMath::Lerp(PreviousTransformKey->Rotation, NextTransformKey->Rotation, lerpValue));
	EchoActor->SetActorScale3D(FMath::Lerp(PreviousTransformKey->Scale, NextTransformKey->Scale, lerpValue));
}

void FEchoTimeline::ActivateTimeline(bool bInIsActive)
{
	if (IsValid(EchoActor))
	{
		bIsActive = bInIsActive;
		EchoActor->SetActorHiddenInGame(bInIsActive);
	}
}

#pragma endregion

#pragma region GlobalTimeline

void FGlobalTimeline::Play(const float& PreviousTimeKey, const float& CurrentTimeKey, bool bIsInRewind, bool& bOutHasReachedEnd)
{
	//TODO: Add Rewind behavior
	
	bool bHasActiveTimeline = false;
	
	for (FEchoTimeline& Timeline : Timelines)
	{
		// Activate or deactivate Timeline
		float LocalTimeKey = CurrentTimeKey - Timeline.StartTimeKey;
		if (Timeline.bIsActive)
		{
			if (Timeline.GetLastTimeKey() < LocalTimeKey)
			{
				Timeline.ActivateTimeline(false);
				continue;
			}
		}
		else if (LocalTimeKey > 0 && LocalTimeKey < Timeline.GetLastTimeKey())
		{
			Timeline.ActivateTimeline(true);
		}
		else
		{
			continue;
		}
		
		// Play Timeline
		bHasActiveTimeline = true;
		Timeline.PlayReplay(PreviousTimeKey, CurrentTimeKey, bIsInRewind);
	}
	
	bOutHasReachedEnd = !bHasActiveTimeline;
}

bool FGlobalTimeline::HasAvailableTimelineSlot() const
{
	return Timelines.Num() < MaxSlots;
}

void FGlobalTimeline::RegisterTimeline(const FEchoTimeline& Timeline)
{
	if (!HasAvailableTimelineSlot()) return;
	Timelines.Add(Timeline);
}

#pragma endregion

void URecordManagerSubsystem::StartRecord(AActor* InRecordedActor)
{
	if (bIsRecording || !GlobalTimeline.HasAvailableTimelineSlot()) return;
	if (!IsValid(InRecordedActor)) return;
	RecordedActor = InRecordedActor;
	bIsRecording = true;
	RecordingTimeline = FEchoTimeline();
	RecordingTimeline.StartTimeKey = CurrentTimeKey;
	RecordingTimeline.RecordTransformKey(RecordedActor, 0);
	OnStartRecording.Broadcast(CurrentTimeKey);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.9f);
}

void URecordManagerSubsystem::StopRecord()
{
	if (bIsRecording)
	{
		bIsRecording = false;
		RecordingTimeline.RecordTransformKey(RecordedActor, CurrentTimeKey);
		RecordedActor = nullptr;
		GlobalTimeline.RegisterTimeline(RecordingTimeline);
		OnStopRecording.Broadcast();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.
void URecordManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}