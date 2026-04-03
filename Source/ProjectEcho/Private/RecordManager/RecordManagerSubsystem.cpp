#include <RecordManager/RecordManagerSubsystem.h>
#include <string>

#include "DataAssetDeveloperSettings.h"
#include "EchoSystem.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/Debug/EchoDebug.h"

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

const float& FEchoTimeline::GetLastTimeKey() const
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
		EchoActor->SetActorHiddenInGame(!bInIsActive);
	}
}

void FEchoTimeline::OnDestroy()
{
	ActivateTimeline(false);
}

#pragma endregion

#pragma region GlobalTimeline

void FGlobalTimeline::Initiate(const int InNbSlots)
{
	if (!Timelines.IsEmpty()) Timelines.Empty();
	NbSlots = InNbSlots;
}

void FGlobalTimeline::Play(const float& PreviousTimeKey, const float& CurrentTimeKey, bool bIsInRewind, bool& bOutHasReachedEnd)
{
	//TODO: Add Rewind behavior
	
	bool bHasNotReachedEnd = false;

	for (int i = 0; i < NbSlots; ++i)
	{
		if (!Timelines.Contains(i)) continue;
		
		// Activate or deactivate Timeline
		float LocalTimeKey = CurrentTimeKey - Timelines[i].StartTimeKey;
		if (Timelines[i].bIsActive)
		{
			if (Timelines[i].GetLastTimeKey() < LocalTimeKey)
			{
				//Timeline Desactivation (Reached End of Last Key)
				Timelines[i].ActivateTimeline(false);
				continue;
			}
		}
		else if (LocalTimeKey < Timelines[i].GetLastTimeKey())
		{
			if (LocalTimeKey > 0)
			{
				//Timeline Activation (Reached FirstKey)
				Timelines[i].ActivateTimeline(true);
			}
			else
			{
				//Has not reached first Key (GlobalTimeline has not reached end)
				bHasNotReachedEnd = true;
				continue;
			}
		}
		else
		{
			continue;
		}
		
		// Play Timeline
		bHasNotReachedEnd = true;
		Timelines[i].PlayReplay(PreviousTimeKey - Timelines[i].StartTimeKey, LocalTimeKey, bIsInRewind);
	}
	
	bOutHasReachedEnd = !bHasNotReachedEnd;
}

bool FGlobalTimeline::HasAvailableTimelineSlot() const
{
	for (int i = 0; i < NbSlots; ++i)
	{
		if (!Timelines.Contains(i)) return true;
	}
	return false;
}

float FGlobalTimeline::GetLastTimeKey() const
{
	float globalLastTimeKey = 0;
	for (int i = 0; i < NbSlots; ++i)
	{
		if (!Timelines.Contains(i)) continue;
		float lastTimeKey = Timelines[i].GetLastTimeKey() + Timelines[i].StartTimeKey;
		if (lastTimeKey > globalLastTimeKey) globalLastTimeKey = lastTimeKey;
	}
	return globalLastTimeKey;
}

void FGlobalTimeline::RegisterTimeline(const FEchoTimeline& Timeline)
{
	if (!HasAvailableTimelineSlot()) return;
	for (int i = 0; i < NbSlots; ++i)
	{
		if (!Timelines.Contains(i))
		{
			Timelines.Add(i, Timeline);
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Registered Timeline at Slot : " + FString::FromInt(i), FColor::Turquoise, 3.f);
			return;
		}
	}
}

void FGlobalTimeline::DestroyTimeline(int SelectedSlot, TArray<TObjectPtr<AActor>>& OutEchoActorPool)
{
	if (Timelines.Contains(SelectedSlot))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Deleted Timeline at Slot : " + FString::FromInt(SelectedSlot), FColor::Turquoise, 3.f);
		
		float StartTimeKey = Timelines[SelectedSlot].StartTimeKey;
		
		//Remove Timeline
		Timelines[SelectedSlot].OnDestroy();
		OutEchoActorPool.Add(Timelines[SelectedSlot].EchoActor);
		Timelines.Remove(SelectedSlot);
		
		if (StartTimeKey == 0.0f && !Timelines.IsEmpty())
		{
			float ClosestStartKey = -1;
			for (int i = 0; i < NbSlots; ++i)
			{
				if (Timelines.Contains(i) && (ClosestStartKey < 0 || Timelines[i].StartTimeKey < ClosestStartKey))
				{
					ClosestStartKey = Timelines[i].StartTimeKey;
				}
			}
			for (int i = 0; i < NbSlots; ++i)
			{
				if (Timelines.Contains(i))
				{
					Timelines[i].StartTimeKey -= ClosestStartKey;
				}
			}
		}
	}
}

#pragma endregion

TStatId URecordManagerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URecordManagerSubsystem, STATGROUP_Tickables);
}

void URecordManagerSubsystem::InitRecordManager(const int& NbTimelineSlot)
{
	
	CurrentTimeKey = 0.0f;
	GlobalTimeline.Initiate(NbTimelineSlot);
	RecordManagerSettings = GetDefault<UDataAssetDeveloperSettings>()->RecordManagerSettings.LoadSynchronous();
	if (!IsValid(RecordManagerSettings))
	{
		UEchoDebug::Log(EEchoSystem::Record, EEchoMessageType::Error, "Missing RecordManagerSettings");
		return;
	}
	TSubclassOf<AActor> EchoActorClass = RecordManagerSettings->EchoActorClass;
	FTransform SpawnTransform;
	for (int i = 0; i < NbTimelineSlot; ++i)
	{
		AActor* SpawnedEchoActor = GetWorld()->SpawnActorDeferred<AActor>(EchoActorClass, SpawnTransform);
		SpawnedEchoActor->SetActorHiddenInGame(true);
		SpawnedEchoActor->FinishSpawning(SpawnTransform);
		EchoActorsPool.Add(SpawnedEchoActor);
	}
}

void URecordManagerSubsystem::StartRecord(AActor* InRecordedActor)
{
	if (bIsRecording || !GlobalTimeline.HasAvailableTimelineSlot()) return;
	if (!IsValid(InRecordedActor)) return;
	if (EchoActorsPool.IsEmpty()) return;
	RecordedActor = InRecordedActor;
	bIsRecording = true;
	RecordingTimeline = FEchoTimeline();
	RecordingTimeline.StartTimeKey = CurrentTimeKey;
	RecordingTimeline.EchoActor = EchoActorsPool.Pop();
	RecordingTimeline.RecordTransformKey(RecordedActor, 0);
	OnStartRecording.Broadcast(CurrentTimeKey);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), RecordManagerSettings->TimeDilatationFactor);
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Start Recording", FColor::Turquoise, 3.f);
}

void URecordManagerSubsystem::StopRecord()
{
	if (bIsRecording)
	{
		bIsRecording = false;
		RecordingTimeline.RecordTransformKey(RecordedActor, CurrentTimeKey - RecordingTimeline.StartTimeKey);
		RecordedActor = nullptr;
		GlobalTimeline.RegisterTimeline(RecordingTimeline);
		OnStopRecording.Broadcast();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Stop Recording", FColor::Turquoise, 3.f);
	}
}

void URecordManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GlobalTimeline.Timelines.IsEmpty() && !bIsRecording) return;
	
	float previousTimeKey = CurrentTimeKey;
	CurrentTimeKey+= DeltaTime;
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Current Time Key  " + FString::SanitizeFloat(CurrentTimeKey), FColor::Cyan, DeltaTime);
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Current Selected Timeline : " + FString::FromInt(SelectedSlot), FColor::Cyan, DeltaTime);
	
	//--- Handle Replay ---
	if (!GlobalTimeline.Timelines.IsEmpty())
	{
		bool bHasReachedEnd = false;
		GlobalTimeline.Play(previousTimeKey, CurrentTimeKey, false, bHasReachedEnd);
		
		if (bHasReachedEnd && !bIsRecording)
		{
			//TODO: Implement Rewind
			//Reset to start of Timeline
			CurrentTimeKey = 0.f;
		}
	}
	
	// --- Handle Recording ---
	if (bIsRecording)
	{
		RecordingTimeline.RecordTransformKey(RecordedActor, CurrentTimeKey - RecordingTimeline.StartTimeKey);
		
		//TODO: Implement Action Keys
	}
	
}

void URecordManagerSubsystem::DestroySelectedTimeline()
{
	GlobalTimeline.DestroyTimeline(SelectedSlot, EchoActorsPool);
	if (GlobalTimeline.Timelines.IsEmpty())
	{
		CurrentTimeKey = 0.0f;
	}
}

void URecordManagerSubsystem::IncrementSelectedSlot()
{
	SelectedSlot++;
	if (SelectedSlot >= GlobalTimeline.NbSlots) SelectedSlot = 0;
}

void URecordManagerSubsystem::DecrementSelectedSlot()
{
	SelectedSlot--;
	if (SelectedSlot < 0) SelectedSlot = GlobalTimeline.NbSlots - 1;
}
