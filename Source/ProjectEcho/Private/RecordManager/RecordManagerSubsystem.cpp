#include <RecordManager/RecordManagerSubsystem.h>
#include <string>

#include "DataAssetDeveloperSettings.h"
#include "EchoSystem.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "RecordManager/EchoActor.h"
#include "RecordManager/RecordableComponent.h"
#include "RecordManager/RecordableInterface.h"
#include "RecordManager/RecordHandlerInterface.h"
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

bool FEchoTimeline::GetActionKeys(const float& PreviousKey,const float& CurrentTimeKey, bool bIsInRewind, TArray<FRecordActionKey>& OutActionKeys) const
{
	bool bHasAddedActionKeys = false;
	for (const FRecordActionKey& ActionKey: bIsInRewind ? RewindActionKeys : ActionKeys)
	{
		if (ActionKey.TimeKey > PreviousKey && ActionKey.TimeKey <= CurrentTimeKey)
		{
			OutActionKeys.Add(ActionKey);
			bHasAddedActionKeys = true;
		}
	}
	return bHasAddedActionKeys;
}

void FEchoTimeline::RegisterEchoActor(AEchoActor* InEchoActor)
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
	if (RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass()))
	{
		TransformKey.ControlRotation = IRecordHandlerInterface::Execute_GetToRecordControlRotation(RecordedActor);
	};
	
	//Add it to the list and Sort the list (list has to be in order)
	TransformKeys.Add(TransformKey);
	TransformKeys.Sort([](const FRecordTransformKey& A, const FRecordTransformKey& B)
	{
		return A.TimeKey < B.TimeKey;
	});
}

void FEchoTimeline::RecordActionKey(AActor* RecordedActor, const float& CurrentTimeKey)
{
	if (!RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass())) return;
	IRecordHandlerInterface* RecordHandlerInterface = Cast<IRecordHandlerInterface>(RecordedActor);
	if (RecordHandlerInterface == nullptr) return;
	
	TArray<TSharedPtr<FRecordedAction>> ToRecordActions = RecordHandlerInterface->GetToRecordActions();
	for (const TSharedPtr<FRecordedAction> ToRecordAction : ToRecordActions)
	{
		FRecordActionKey RecordActionKey;
		RecordActionKey.TimeKey = CurrentTimeKey;
		RecordActionKey.Action = ToRecordAction;
		ActionKeys.Add(RecordActionKey);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Recording Action : " + UEnum::GetDisplayValueAsText(ToRecordAction->ActionEnum).ToString());
	}
	
	TArray<TSharedPtr<FRecordedAction>> ToRecordRewindActions = RecordHandlerInterface->GetToRecordRewindActions();
	for (const TSharedPtr<FRecordedAction> ToRecordAction : ToRecordRewindActions)
	{
		FRecordActionKey RecordActionKey;
		RecordActionKey.TimeKey = CurrentTimeKey;
		RecordActionKey.Action = ToRecordAction;
		RewindActionKeys.Add(RecordActionKey);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Recording Rewind Action : " + UEnum::GetDisplayValueAsText(ToRecordAction->ActionEnum).ToString());
	}
}

void FEchoTimeline::PlayReplay(const float& PreviousKey,const float& CurrentTimeKey, bool bIsInRewind)
{
	if (!IsValid(EchoActor)) return;
	if (GetLastTimeKey() < CurrentTimeKey) return;
	
	//Play Transform Key (Lerp between two closest Keys)
	const FRecordTransformKey* PreviousTransformKey = GetPreviousTransformKey(CurrentTimeKey);
	const FRecordTransformKey* NextTransformKey = GetNextTransformKey(CurrentTimeKey);
	if (NextTransformKey == nullptr || PreviousTransformKey == nullptr) return;
	
	//Place Actor according to previous and next TransformKey 
	float lerpValue = (CurrentTimeKey - PreviousTransformKey->TimeKey) / (NextTransformKey->TimeKey - PreviousTransformKey->TimeKey);
	EchoActor->SetActorLocation(FMath::Lerp(PreviousTransformKey->Position, NextTransformKey->Position, lerpValue));
	EchoActor->SetActorRotation(FMath::Lerp(PreviousTransformKey->Rotation, NextTransformKey->Rotation, lerpValue));
	EchoActor->SetActorScale3D(FMath::Lerp(PreviousTransformKey->Scale, NextTransformKey->Scale, lerpValue));
	EchoActor->SetControlRotation(FMath::Lerp(PreviousTransformKey->ControlRotation, NextTransformKey->ControlRotation, lerpValue));
	
	//Play Action Keys
	TArray<FRecordActionKey> CurrentActionKeys;
	if (GetActionKeys(PreviousKey, CurrentTimeKey, bIsInRewind, CurrentActionKeys))
	{
		for (const FRecordActionKey& ActionKey : CurrentActionKeys)
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Replaying Action : " + UEnum::GetDisplayValueAsText(ActionKey.Action->ActionEnum).ToString());
			EchoActor->HandleActionKey(ActionKey.Action->ActionEnum);
		}
	}
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
	bool bHasNotReachedEnd = false;

	for (int i = 0; i < NbSlots; ++i)
	{
		if (!Timelines.Contains(i)) continue;
		
		// Activate or deactivate Timeline
		float LocalTimeKey = CurrentTimeKey - Timelines[i].StartTimeKey;
		if (Timelines[i].bIsActive)
		{
			if (bIsInRewind? LocalTimeKey < 0 : Timelines[i].GetLastTimeKey() < LocalTimeKey)
			{
				//Timeline Desactivation (Reached End of Last Key // In Rewind, Reached passed FirstKey)
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
	
	bOutHasReachedEnd = bIsInRewind ? false : !bHasNotReachedEnd;
}

bool FGlobalTimeline::HasAvailableTimelineSlot() const
{
	for (int i = 0; i < NbSlots; ++i)
	{
		if (!Timelines.Contains(i)) return true;
	}
	return false;
}

int FGlobalTimeline::GetFirstAvailableTimelineSlot() const
{
	for (int i = 0; i < NbSlots; ++i)
	{
		if (!Timelines.Contains(i)) return i;
	}
	return -1;
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

float FGlobalTimeline::GetLength() const
{
	float length = 0;
	for (int i = 0; i < NbSlots; ++i)
	{
		if (!Timelines.Contains(i)) continue;
		float lastTimeKey = Timelines[i].GetLastTimeKey() + Timelines[i].StartTimeKey;
		if (lastTimeKey > length) length = lastTimeKey;
	}
	return length;
}

void FGlobalTimeline::RegisterTimeline(const int& TimelineIndex, const FEchoTimeline& Timeline, TObjectPtr<URecordManagerSettings> Settings)
{
	if (!HasAvailableTimelineSlot()) return;
	if (!Timelines.Contains(TimelineIndex))
	{
		if (Settings->EchoColors.Contains(TimelineIndex))
		{
			Timeline.EchoActor->InitEcho(TimelineIndex, Settings->EchoColors[TimelineIndex]);
		}
		Timelines.Add(TimelineIndex, Timeline);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Registered Timeline at Slot : " + FString::FromInt(TimelineIndex), FColor::Turquoise, 3.f);
	}
}

void FGlobalTimeline::DestroyTimeline(int SelectedSlot, TArray<TObjectPtr<AEchoActor>>& OutEchoActorPool)
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
		AEchoActor* SpawnedEchoActor = GetWorld()->SpawnActorDeferred<AEchoActor>(EchoActorClass, SpawnTransform);
		SpawnedEchoActor->SetActorHiddenInGame(true);
		SpawnedEchoActor->FinishSpawning(SpawnTransform);
		EchoActorsPool.Add(SpawnedEchoActor);
	}
	
	//Find All Recordables
	TArray<AActor*> RecordableActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), URecordableInterface::StaticClass(), RecordableActors);
	for (AActor* RecordableActor : RecordableActors)
	{
		if (RecordableActor != nullptr)
		{
			if (URecordableComponent* RecordableComponent = RecordableActor->GetComponentByClass<URecordableComponent>(); RecordableComponent != nullptr)
			{
				RecordableComponents.Add(RecordableComponent);
				RecordableComponent->OnInteracted.AddDynamic(this, &URecordManagerSubsystem::OnRecordableInteractedWith);
			}
		}
	}
}

void URecordManagerSubsystem::StartRecord(AActor* InRecordedActor)
{
	if (!CanStartRecord()) return;
	if (!IsValid(InRecordedActor)) return;
	if (EchoActorsPool.IsEmpty()) return;
	RecordedActor = InRecordedActor;
	bIsRecording = true;
	CurrentRecordingTimelineIndex = GlobalTimeline.GetFirstAvailableTimelineSlot();
	if (RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass()))
	{
		IRecordHandlerInterface::Execute_StartRecording(RecordedActor);
	}
	RecordingTimeline = FEchoTimeline();
	RecordingTimeline.StartTimeKey = CurrentTimeKey;
	RecordingTimeline.EchoActor = EchoActorsPool.Pop();
	RecordingTimeline.RecordTransformKey(RecordedActor, 0);
	OnStartRecording.Broadcast(CurrentTimeKey, CurrentRecordingTimelineIndex, RecordManagerSettings->EchoColors[CurrentRecordingTimelineIndex]);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), RecordManagerSettings->TimeDilatationFactor);
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Start Recording", FColor::Turquoise, 3.f);
}

bool URecordManagerSubsystem::CanStartRecord() const
{
	return !bIsRecording && GlobalTimeline.HasAvailableTimelineSlot() && !bIsInRewind;
}

bool URecordManagerSubsystem::CanStopRecord() const
{
	return bIsRecording && (CurrentTimeKey - RecordingTimeline.StartTimeKey) > RecordManagerSettings->MinRecordTime;
}

void URecordManagerSubsystem::StopRecord()
{
	if (CanStopRecord())
	{
		bIsRecording = false;
		if (RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass()))
		{
			IRecordHandlerInterface::Execute_StartRecording(RecordedActor);
		}
		RecordingTimeline.RecordTransformKey(RecordedActor, CurrentTimeKey - RecordingTimeline.StartTimeKey);
		OnStopRecording.Broadcast();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Stop Recording", FColor::Turquoise, 3.f);
	
		//Start Player Rewind
		StartPlayerRewind();
	}
}

void URecordManagerSubsystem::StartPlayerRewind()
{
	StartRewind();
	bIsPlayerRewinding = true;
	RewindSpeed = RecordingTimeline.GetLastTimeKey() / RecordManagerSettings->PlayerRewindTime;
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Start PlayerRewind at Speed : " + FString::SanitizeFloat(RewindSpeed), FColor::Turquoise, 3.f);
	OnStartPlayerRewinding.Broadcast();
}

void URecordManagerSubsystem::StopPlayerRewind()
{
	//Set Actor to Start Position
	RecordedActor->SetActorLocation(RecordingTimeline.TransformKeys[0].Position);
	RecordedActor->SetActorRotation(RecordingTimeline.TransformKeys[0].Rotation);
	RecordedActor->SetActorScale3D(RecordingTimeline.TransformKeys[0].Scale);
	if (RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass()))
	{
		IRecordHandlerInterface::Execute_SetControlRotation(RecordedActor, RecordingTimeline.TransformKeys[0].ControlRotation);
	}
	RecordedActor = nullptr;
	
	GlobalTimeline.RegisterTimeline(CurrentRecordingTimelineIndex, RecordingTimeline, RecordManagerSettings);
	OnStopPlayerRewinding.Broadcast();
	
	StopRewind();
	bIsPlayerRewinding = false;
}

void URecordManagerSubsystem::StartRewind()
{
	bIsInRewind = true;
	for (TObjectPtr<URecordableComponent> RecordableComponent : RecordableComponents)
	{
		if (!IsValid(RecordableComponent)) continue;
		if (RecordableComponent->IsRecording())
		{
			RecordableComponent->StartRewind();
		}
	}
}

void URecordManagerSubsystem::StopRewind()
{
	bIsInRewind = false;
	for (TObjectPtr<URecordableComponent> RecordableComponent : RecordableComponents)
	{
		if (!IsValid(RecordableComponent)) return;
		if (RecordableComponent->IsRecording())
		{
			RecordableComponent->StopRewind(CurrentTimeKey);
		}
	}
}

bool URecordManagerSubsystem::IsRecording()
{
	return bIsRecording;
}

void URecordManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GlobalTimeline.Timelines.IsEmpty() && !bIsRecording && !bIsPlayerRewinding) return;
	
	float previousTimeKey = CurrentTimeKey;
	CurrentTimeKey += DeltaTime * (bIsInRewind? -RewindSpeed : 1);
	CurrentTimeKey = FMath::Max(CurrentTimeKey, 0);
	
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Current Time Key : " + FString::SanitizeFloat(CurrentTimeKey), FColor::Cyan, DeltaTime);
	if (bIsInRewind) UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Rewinding at Speed : " + FString::SanitizeFloat(RewindSpeed), FColor::Cyan, DeltaTime);
	UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Current Selected Timeline : " + FString::FromInt(SelectedSlot), FColor::Cyan, DeltaTime);
	
	//Handle Recordables
	if (bIsInRewind)
	{
		for (TObjectPtr<URecordableComponent> RecordableComponent : RecordableComponents)
		{
			if (!IsValid(RecordableComponent)) continue;
			if (RecordableComponent->IsRecording())
			{
				if (CurrentTimeKey > RecordableComponent->GetFirstInteractedKey())
				{
					RecordableComponent->ReplayKey(previousTimeKey, CurrentTimeKey);
				}
				else
				{
					RecordableComponent->ReplayFirstKey();
					RecordableComponent->StopRewind(CurrentTimeKey);
					RecordableComponent->StopRecording();
				}
			}
		}
	}
	else
	{
		for (TObjectPtr<URecordableComponent> RecordableComponent : RecordableComponents)
		{
			if (!IsValid(RecordableComponent)) continue;
			if (RecordableComponent->IsRecording())
			{
				RecordableComponent->RecordKey(CurrentTimeKey);
			}
		}
	}
	
	//--- Handle Replay ---
	if (!GlobalTimeline.Timelines.IsEmpty() || bIsPlayerRewinding)
	{
		bool bHasReachedEnd = false;
		GlobalTimeline.Play(previousTimeKey, CurrentTimeKey, bIsInRewind, bHasReachedEnd);
		
		//Handle Player Rewinding
		if (bIsPlayerRewinding)
		{
			if (RecordingTimeline.StartTimeKey >= CurrentTimeKey)
			{
				StopPlayerRewind();
				return;
			}
			PlayPlayerRewind(CurrentTimeKey - RecordingTimeline.StartTimeKey);
		}
		
		if (bHasReachedEnd && !bIsRecording && !bIsInRewind)
		{
			RewindSpeed = GlobalTimeline.GetLength() / RecordManagerSettings->GlobalRewindTime;
			StartRewind();
		}
		else if (bIsInRewind && CurrentTimeKey <= 0)
		{
			CurrentTimeKey = 0;
			StopRewind();
		}
	}
	
	// --- Handle Recording ---
	if (bIsRecording)
	{
		float LocalTimeKey = CurrentTimeKey - RecordingTimeline.StartTimeKey;
		RecordingTimeline.RecordTransformKey(RecordedActor, LocalTimeKey);
		RecordingTimeline.RecordActionKey(RecordedActor, LocalTimeKey);
		
		if (LocalTimeKey >= RecordManagerSettings->MaxRecordTime)
		{
			//Reached MaxRecordTime, Stop Record
			StopRecord();
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Reached Max Record Time", FColor::Turquoise, 3.f);
		}
	}
}

void URecordManagerSubsystem::PlayPlayerRewind(const float& TimeKey)
{
	if (!IsValid(RecordedActor)) return;
	
	const FRecordTransformKey* PreviousTransformKey = RecordingTimeline.GetPreviousTransformKey(TimeKey);
	const FRecordTransformKey* NextTransformKey = RecordingTimeline.GetNextTransformKey(TimeKey);
	if (NextTransformKey != nullptr && PreviousTransformKey != nullptr)
	{
		//Place RecordedActor according to previous and next TransformKey
		float lerpValue = (TimeKey - PreviousTransformKey->TimeKey) / (NextTransformKey->TimeKey - PreviousTransformKey->TimeKey);
		RecordedActor->SetActorLocation(FMath::Lerp(PreviousTransformKey->Position, NextTransformKey->Position, lerpValue));
		RecordedActor->SetActorRotation(FMath::Lerp(PreviousTransformKey->Rotation, NextTransformKey->Rotation, lerpValue));
		RecordedActor->SetActorScale3D(FMath::Lerp(PreviousTransformKey->Scale, NextTransformKey->Scale, lerpValue));
		if (RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass()))
		{
			IRecordHandlerInterface::Execute_SetControlRotation(RecordedActor, FMath::Lerp(PreviousTransformKey->ControlRotation, NextTransformKey->ControlRotation, lerpValue));
		}
	}
}

void URecordManagerSubsystem::DestroySelectedTimeline()
{
	if (bIsInRewind) return; //Forbid Timeline Destruction during Rewind
	GlobalTimeline.DestroyTimeline(SelectedSlot, EchoActorsPool);
	if (GlobalTimeline.Timelines.IsEmpty())
	{
		CurrentTimeKey = 0.0f;
		
		//Reset Recordable to origin Positions
		for (TObjectPtr<URecordableComponent> RecordableComponent : RecordableComponents)
		{
			RecordableComponent->StartRewind();
			RecordableComponent->ReplayFirstKey();
			RecordableComponent->StopRewind(CurrentTimeKey);
			RecordableComponent->StopRecording();
		}
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

void URecordManagerSubsystem::OnRecordableInteractedWith(URecordableComponent* Self, bool bShouldRecord)
{
	if (bShouldRecord)
	{
		UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Starting to Record " + Self->GetOwner()->GetName(), FColor::Turquoise, 2.f);
		Self->StartRecording(CurrentTimeKey);
		Self->RecordKey(CurrentTimeKey);
	}
	else
	{
		UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Stopping to Record " + Self->GetOwner()->GetName(), FColor::Turquoise, 2.f);
		Self->StopRecording();
	}
}
