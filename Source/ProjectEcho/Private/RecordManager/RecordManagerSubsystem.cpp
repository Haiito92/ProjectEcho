#include <RecordManager/RecordManagerSubsystem.h>
#include <string>

#include "DataAssetDeveloperSettings.h"
#include "EchoSystem.h"
#include "ProjectEcho.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "LevelStreaming/LevelStreamingWorldSubsystem.h"
#include "RecordManager/EchoActor.h"
#include "RecordManager/RecordableComponent.h"
#include "RecordManager/RecordableInterface.h"
#include "RecordManager/RecordHandlerInterface.h"
#include "RecordManager/RecordListener.h"
#include "Tools/Debug/EchoDebug.h"

#pragma region Timeline

const FRecordTransformKey* FEchoTimeline::GetNextTransformKey(const float& TimeKey) const
{
	for (const FRecordTransformKey& TransformKey : TransformKeys)
	{
		if (TransformKey.TimeKey > TimeKey)
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

float FEchoTimeline::GetLastTimeKey() const
{
	if (TransformKeys.IsEmpty()) return 0;
	return TransformKeys[TransformKeys.Num() - 1].TimeKey;
}

bool FEchoTimeline::GetActionKeys(const float& PreviousKey,const float& CurrentTimeKey, bool bIsInRewind, TArray<FRecordActionKey>& OutActionKeys) const
{
	bool bHasAddedActionKeys = false;
	if (!bIsInRewind)
	{
		for (const FRecordActionKey& ActionKey : ActionKeys)
		{
			if (ActionKey.TimeKey > PreviousKey && ActionKey.TimeKey <= CurrentTimeKey)
			{
				OutActionKeys.Add(ActionKey);
				bHasAddedActionKeys = true;
			}
		}
	}
	else
	{
		for (const FRecordActionKey& ActionKey : RewindActionKeys)
		{
			if (ActionKey.TimeKey < PreviousKey && ActionKey.TimeKey >= CurrentTimeKey)
			{
				UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Previous Key : " + FString::SanitizeFloat(PreviousKey) + ", CurrentKey = " + FString::SanitizeFloat(CurrentTimeKey) + ", ActionKey = " + FString::SanitizeFloat(ActionKey.TimeKey), FColor::Turquoise, 1);
				OutActionKeys.Add(ActionKey);
				bHasAddedActionKeys = true;
			}
		}
	}
	return bHasAddedActionKeys;
}

bool FEchoTimeline::GetAnimationKeys(const float& PreviousKey, const float& CurrentTimeKey,
	TArray<FRecordAnimationKey>& OutAnimationKeys) const
{
	bool bHasAddedAnimationKey = false;
	for (const FRecordAnimationKey& AnimationKey : RecordAnimationKeys)
	{
		if (AnimationKey.TimeKey > PreviousKey && AnimationKey.TimeKey <= CurrentTimeKey)
		{
			UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Previous Key : " + FString::SanitizeFloat(PreviousKey) + ", CurrentKey = " + FString::SanitizeFloat(CurrentTimeKey) + ", AnimationKey = " + FString::SanitizeFloat(AnimationKey.TimeKey), FColor::Turquoise, 1);
			OutAnimationKeys.Add(AnimationKey);
			bHasAddedAnimationKey = true;
		}
	}
	return bHasAddedAnimationKey;
}

void FEchoTimeline::RestoreAnimationKeys(const float& CurrentTimeKey)
{
	for (const FRecordAnimationValue& AnimationValue : DefaultAnimationValues)
	{
		FRecordAnimationValue LastAnimationValue = AnimationValue;
		FindLastPlayedAnimationKey(CurrentTimeKey, AnimationValue.AnimationValueReference, LastAnimationValue);
		EchoActor->HandleAnimationKey(LastAnimationValue);
	}
}

bool FEchoTimeline::FindLastPlayedAnimationKey(const float& CurrentTimeKey,
	EAnimationValueReference AnimationValueReference, FRecordAnimationValue& AnimationValue) const
{
	bool bHasKey = false;
	for (const FRecordAnimationKey& RecordAnimationKey : RecordAnimationKeys)
	{
		if (RecordAnimationKey.TimeKey > CurrentTimeKey) return bHasKey;
		if (RecordAnimationKey.RecordAnimationValue.AnimationValueReference == AnimationValueReference)
		{
			bHasKey = true;
			AnimationValue = RecordAnimationKey.RecordAnimationValue;
		}
	}
	return bHasKey;
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

void FEchoTimeline::ReplaceTransformKey(AActor* RecordedActor, const float& CurrentTimeKey, bool bRecordIfNotFound, const FRotator* OverrideControlRotation)
{
	FRecordTransformKey* FoundTransformKey = TransformKeys.FindByPredicate([CurrentTimeKey](const FRecordTransformKey& TransformKey)
	{
		return TransformKey.TimeKey == CurrentTimeKey;
	});
	if (FoundTransformKey == nullptr)
	{
		if (bRecordIfNotFound) RecordTransformKey(RecordedActor, CurrentTimeKey);
		return;
	}
	FoundTransformKey->Position = RecordedActor->GetActorLocation();
	FoundTransformKey->Rotation = RecordedActor->GetActorRotation();
	FoundTransformKey->Scale = RecordedActor->GetActorScale();
	if (OverrideControlRotation != nullptr)
	{
		FoundTransformKey->ControlRotation = *OverrideControlRotation;
	}
	else if (RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass()))
	{
		FoundTransformKey->ControlRotation = IRecordHandlerInterface::Execute_GetToRecordControlRotation(RecordedActor);
	}
}

bool FEchoTimeline::HasTransformKey(const float& CurrentTimeKey) const
{
	const FRecordTransformKey* FoundTransformKey = TransformKeys.FindByPredicate([CurrentTimeKey](const FRecordTransformKey& TransformKey)
	{
		return TransformKey.TimeKey == CurrentTimeKey;
	});
	return FoundTransformKey != nullptr;
}

void FEchoTimeline::RecordActionKey(AActor* RecordedActor, const float& CurrentTimeKey)
{
	if (!RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass())) return;
	IRecordHandlerInterface* RecordHandlerInterface = Cast<IRecordHandlerInterface>(RecordedActor);
	if (RecordHandlerInterface == nullptr) return;
	
	TArray<FRecordedAction> ToRecordActions = RecordHandlerInterface->GetToRecordActions();
	for (const FRecordedAction& ToRecordAction : ToRecordActions)
	{
		FRecordActionKey RecordActionKey;
		RecordActionKey.TimeKey = CurrentTimeKey;
		RecordActionKey.Action = ToRecordAction;
		ActionKeys.Add(RecordActionKey);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Recording Action : " + UEnum::GetDisplayValueAsText(ToRecordAction.ActionEnum).ToString());
	}
	
	TArray<FRecordedAction> ToRecordRewindActions = RecordHandlerInterface->GetToRecordRewindActions();
	for (const FRecordedAction& ToRecordAction : ToRecordRewindActions)
	{
		FRecordActionKey RecordActionKey;
		RecordActionKey.TimeKey = CurrentTimeKey;
		RecordActionKey.Action = ToRecordAction;
		RewindActionKeys.Add(RecordActionKey);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Recording Rewind Action : " + UEnum::GetDisplayValueAsText(ToRecordAction.ActionEnum).ToString());
	}
}

void FEchoTimeline::RecordAnimationKey(AActor* RecordedActor, const float& CurrentTimeKey)
{
	if (!RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass())) return;
	IRecordHandlerInterface* RecordHandlerInterface = Cast<IRecordHandlerInterface>(RecordedActor);
	if (RecordHandlerInterface == nullptr) return;
	
	TArray<FRecordAnimationValue> ToRecordAnimationKeys = RecordHandlerInterface->GetToRecordAnimationKeys();
	for (FRecordAnimationValue& ToRecordAnimationKey : ToRecordAnimationKeys)
	{
		RecordAnimationKeys.Add(FRecordAnimationKey(ToRecordAnimationKey, CurrentTimeKey));
	}
	
	//Sort Animation Keys
	RecordAnimationKeys.Sort([](const FRecordAnimationKey& A, const FRecordAnimationKey& B)
	{
		return A.TimeKey < B.TimeKey;
	});
}

void FEchoTimeline::PlayReplay(const float& PreviousKey,const float& CurrentTimeKey, bool bIsInRewind)
{
	if (!IsValid(EchoActor)) return;
	if (GetLastTimeKey() < CurrentTimeKey) return;
	
	PlayTransformKeys(CurrentTimeKey);
	
	//Play Action Keys
	TArray<FRecordActionKey> CurrentActionKeys;
	if (GetActionKeys(PreviousKey, CurrentTimeKey, bIsInRewind, CurrentActionKeys))
	{
		for (const FRecordActionKey& ActionKey : CurrentActionKeys)
		{
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Replaying Action : " + UEnum::GetDisplayValueAsText(ActionKey.Action.ActionEnum).ToString());
			EchoActor->HandleActionKey(ActionKey.Action);
		}
	}
	
	if (!bIsInRewind)
	{
		TArray<FRecordAnimationKey> CurrentAnimationKeys;
		if (GetAnimationKeys(PreviousKey, CurrentTimeKey, CurrentAnimationKeys))
		{
			for (const FRecordAnimationKey& AnimationKey : CurrentAnimationKeys)
			{
				EchoActor->HandleAnimationKey(AnimationKey.RecordAnimationValue);
			}
		}
	}
}

void FEchoTimeline::PlayTransformKeys(const float& CurrentTimeKey)
{
	//Play Transform Key (Lerp between two closest Keys)
	const FRecordTransformKey* PreviousTransformKey = GetPreviousTransformKey(CurrentTimeKey);
	const FRecordTransformKey* NextTransformKey = GetNextTransformKey(CurrentTimeKey);
	if (PreviousTransformKey == nullptr) return;
	if (NextTransformKey == nullptr)
	{
		EchoActor->SetActorLocation(PreviousTransformKey->Position);
		EchoActor->SetActorRotation(PreviousTransformKey->Rotation);
		EchoActor->SetActorScale3D(PreviousTransformKey->Scale);
		EchoActor->SetControlRotation(PreviousTransformKey->ControlRotation);
		return;
	};
	
	//Place Actor according to previous and next TransformKey 
	float lerpValue = (CurrentTimeKey - PreviousTransformKey->TimeKey) / (NextTransformKey->TimeKey - PreviousTransformKey->TimeKey);
	EchoActor->SetActorLocation(FMath::Lerp(PreviousTransformKey->Position, NextTransformKey->Position, lerpValue));
	EchoActor->SetActorRotation(FMath::Lerp(PreviousTransformKey->Rotation, NextTransformKey->Rotation, lerpValue));
	EchoActor->SetActorScale3D(FMath::Lerp(PreviousTransformKey->Scale, NextTransformKey->Scale, lerpValue));
	EchoActor->SetControlRotation(FMath::Lerp(PreviousTransformKey->ControlRotation, NextTransformKey->ControlRotation, lerpValue));
}

FRotator FEchoTimeline::GetControlRotationOfCurrentKey(const float& CurrentTimeKey)
{
	const FRecordTransformKey* PreviousTransformKey = GetPreviousTransformKey(CurrentTimeKey);
	const FRecordTransformKey* NextTransformKey = GetNextTransformKey(CurrentTimeKey);
	
	if (PreviousTransformKey == nullptr) return FRotator();
	if (NextTransformKey == nullptr) return PreviousTransformKey->ControlRotation;

	float lerpValue = (CurrentTimeKey - PreviousTransformKey->TimeKey) / (NextTransformKey->TimeKey - PreviousTransformKey->TimeKey);
	return FMath::Lerp(PreviousTransformKey->ControlRotation, NextTransformKey->ControlRotation, lerpValue);
}

void FEchoTimeline::PlayFirstKey(TArray<FRecordedAction> RestoreFirstStateAction)
{
	if (!IsValid(EchoActor)) return;
	FRecordTransformKey RecordTransformKey = TransformKeys[0];
	EchoActor->SetActorLocation(RecordTransformKey.Position);
	EchoActor->SetActorRotation(RecordTransformKey.Rotation);
	EchoActor->SetActorScale3D(RecordTransformKey.Scale);
	EchoActor->SetControlRotation(RecordTransformKey.ControlRotation);

	for (const FRecordedAction& RecordedAction : RestoreFirstStateAction)
	{
		EchoActor->HandleActionKey(RecordedAction);
	}
	
	RestoreAnimationKeys(0);
}

void FEchoTimeline::ActivateTimeline(bool bInIsActive)
{
	if (IsValid(EchoActor))
	{
		bIsActive = bInIsActive;
		//EchoActor->SetActorHiddenInGame(!bInIsActive);
	}
}

void FEchoTimeline::HandleRewindStarted(const float& CurrentTimeKey, bool bIsPlayerRewind)
{
	EchoActor->HandleRewindStarted(CurrentTimeKey, bIsPlayerRewind);
}

void FEchoTimeline::HandleRewindStopped(const float& CurrentTimeKey, bool bIsPlayerRewind)
{
	//Restore Animation Values to Current Key
	RestoreAnimationKeys(CurrentTimeKey);
	
	//Call BP Function
	EchoActor->HandleRewindStopped(CurrentTimeKey, bIsPlayerRewind);
}

void FEchoTimeline::HandleRecordStarted(const float& CurrentTimeKey)
{
	EchoActor->HandleRecordStarted(CurrentTimeKey);
}

void FEchoTimeline::OnDestroy()
{
	ActivateTimeline(false);
	EchoActor->OnTimelineDestroyed();
	EchoActor->SetActorHiddenInGame(true);
}

void FEchoTimeline::RecordFirstActionKeys(const TArray<FRecordedAction>& FirstActions)
{
	if (FirstActions.IsEmpty()) return;
	for (const FRecordedAction& FirstAction : FirstActions)
	{
		FRecordActionKey ActionKey;
		ActionKey.Action = FirstAction;
		ActionKey.TimeKey = 0.01;
		ActionKeys.Add(ActionKey);
	}
}

void FEchoTimeline::RecordDefaultAnimationValues(AActor* RecordedActor, const float& CurrentTimeKey)
{
	DefaultAnimationValues.Empty();
	if (!RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass())) return;
	IRecordHandlerInterface* RecordHandlerInterface = Cast<IRecordHandlerInterface>(RecordedActor);
	if (RecordHandlerInterface == nullptr) return;
	DefaultAnimationValues = RecordHandlerInterface->GetDefaultAnimationValues();
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
			Timeline.EchoActor->SetActorHiddenInGame(false);
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
		Timelines[SelectedSlot].EchoActor->SetActorLocation(FVector(-100000));
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

void FGlobalTimeline::HandleRewindStarted(const float& CurrentTimeKey, bool bIsPlayerRewind)
{
	for (TTuple<int, FEchoTimeline>& TimelineTuple: Timelines)
	{
		TimelineTuple.Get<1>().HandleRewindStarted(CurrentTimeKey, bIsPlayerRewind);
	}
}

void FGlobalTimeline::HandleRewindStopped(const float& CurrentTimeKey, bool bIsPlayerRewind)
{
	for (TTuple<int, FEchoTimeline>& TimelineTuple: Timelines)
	{
		TimelineTuple.Get<1>().HandleRewindStopped(CurrentTimeKey, bIsPlayerRewind);
	}
}

void FGlobalTimeline::HandleRecordStarted(const float& CurrentTimeKey)
{
	for (TTuple<int, FEchoTimeline>& TimelineTuple: Timelines)
	{
		TimelineTuple.Get<1>().HandleRecordStarted(CurrentTimeKey);
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
		SpawnedEchoActor->OnEchoDestroyed.AddDynamic(this, &URecordManagerSubsystem::OnEchoDestroyed);
		SpawnedEchoActor->SetActorLocation(FVector(-100000));
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
	
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), URecordListener::StaticClass(), RecordListeners);
	
	//Bind to LevelStreaming Functions : 

	ULevelStreamingWorldSubsystem* LevelStreamingWorldSubsystem = GetWorld()->GetSubsystem<ULevelStreamingWorldSubsystem>();
	if (LevelStreamingWorldSubsystem != nullptr)
	{
		LevelStreamingWorldSubsystem->StreamLevelLoaded.AddDynamic(this, &URecordManagerSubsystem::OnNewLevelLoaded);
		LevelStreamingWorldSubsystem->StreamLevelUnloaded.AddDynamic(this, &URecordManagerSubsystem::OnLevelUnloaded);
	}
}

void URecordManagerSubsystem::StartRecord(AActor* InRecordedActor, const TArray<FRecordedAction>& RestoreStateAction,  const TArray<FRecordedAction>& FirstActions)
{
	if (!CanStartRecord()) return;
	if (!IsValid(InRecordedActor)) return;
	if (EchoActorsPool.IsEmpty()) return;
		
	RecordingTimelineStartActions = RestoreStateAction;
	RecordedActor = InRecordedActor;
	bIsRecording = true;
	CurrentRecordingTimelineIndex = GlobalTimeline.GetFirstAvailableTimelineSlot();
	SelectSlot(CurrentRecordingTimelineIndex, true);
	if (RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass()))
	{
		IRecordHandlerInterface::Execute_StartRecording(RecordedActor);
	}
	
	GlobalTimeline.HandleRecordStarted(CurrentTimeKey);

	for (TObjectPtr<URecordableComponent> RecordableComponent : RecordableComponents)
	{
		if (!IsValid(RecordableComponent)) continue;
		if (RecordableComponent->IsCurrentlyInteractedWith() && !RecordableComponent->IsRecording())
		{
			RecordableComponent->StartRecording(FRecordInteractionKey(CurrentTimeKey, CurrentRecordingTimelineIndex));
		}
	}
	
	RecordingTimeline = FEchoTimeline();
	RecordingTimeline.StartTimeKey = CurrentTimeKey;
	RecordingTimeline.EchoActor = EchoActorsPool.Pop();
	RecordingTimeline.RecordTransformKey(RecordedActor, 0);
	RecordingTimeline.RecordFirstActionKeys(FirstActions);
	RecordingTimeline.RecordDefaultAnimationValues(RecordedActor, 0);
	OnStartRecording.Broadcast(CurrentTimeKey, CurrentRecordingTimelineIndex, RecordManagerSettings->EchoColors[CurrentRecordingTimelineIndex]);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), RecordManagerSettings->TimeDilatationFactor);
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Start Recording", FColor::Turquoise, 3.f);
	
	for (AActor* RecordListener : RecordListeners)
	{
		IRecordListener::Execute_ReactToRecordStart(RecordListener);
	}
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
			IRecordHandlerInterface::Execute_StopRecording(RecordedActor);
		}
		RecordingTimeline.RecordTransformKey(RecordedActor, CurrentTimeKey - RecordingTimeline.StartTimeKey);
		OnStopRecording.Broadcast();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Stop Recording", FColor::Turquoise, 3.f);
	
		//Start Player Rewind
		StartPlayerRewind();
		
		for (AActor* RecordListener : RecordListeners)
		{
			IRecordListener::Execute_ReactToRecordEnd(RecordListener);
		}
	}
}

void URecordManagerSubsystem::StartPlayerRewind()
{
	StartRewind(true);
	bIsPlayerRewinding = true;
	RewindSpeed = RecordingTimeline.GetLastTimeKey() / RecordManagerSettings->PlayerRewindTime;
	UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Start PlayerRewind at Speed : " + FString::SanitizeFloat(RewindSpeed), FColor::Turquoise, 3.f);
	OnStartPlayerRewinding.Broadcast();
	
	for (AActor* RecordListener : RecordListeners)
	{
		IRecordListener::Execute_ReactToPlayerRewindStart(RecordListener);
	}
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

	OnStopPlayerRewinding.Broadcast();
	StopRewind(true);
	bIsPlayerRewinding = false;
	
	RecordingTimeline.PlayFirstKey(RecordingTimelineStartActions);
	RecordingTimelineStartActions.Empty();
	GlobalTimeline.RegisterTimeline(CurrentRecordingTimelineIndex, RecordingTimeline, RecordManagerSettings);
	OnTimelineCreated.Broadcast(CurrentRecordingTimelineIndex);
	
	for (AActor* RecordListener : RecordListeners)
	{
		IRecordListener::Execute_ReactToPlayerRewindEnd(RecordListener);
	}
}

void URecordManagerSubsystem::StartRewind(bool bIsPlayerRewind)
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
	
	GlobalTimeline.HandleRewindStarted(CurrentTimeKey, bIsPlayerRewind);
	
	for (AActor* RecordListener : RecordListeners)
	{
		IRecordListener::Execute_ReactToRewindStart(RecordListener);
	}
}

void URecordManagerSubsystem::StopRewind(bool bIsPlayerRewind)
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
	
	GlobalTimeline.HandleRewindStopped(CurrentTimeKey, bIsPlayerRewind);
	
	for (AActor* RecordListener : RecordListeners)
	{
		IRecordListener::Execute_ReactToRewindEnd(RecordListener);
	}
}

bool URecordManagerSubsystem::IsRecording()
{
	return bIsRecording;
}

FGlobalTimelineUIInfo URecordManagerSubsystem::GetGlobalTimelineUIInformation()
{
	FGlobalTimelineUIInfo Info;
	Info.CurrentTimeKey = CurrentTimeKey;
	Info.SelectedTimelineIndex = SelectedSlot;
	Info.Length = GlobalTimeline.GetLength();
	Info.NbSlots = GlobalTimeline.NbSlots;
	Info.Timelines.Empty();
	for (int i = 0; i < GlobalTimeline.NbSlots; ++i)
	{
		if (GlobalTimeline.Timelines.Contains(i))
		{
			Info.Timelines.Add(GetTimelineUIInfo(i));
		}
	}
	Info.Timelines.Sort([&](const FTimelineUIInfo& A,const FTimelineUIInfo& B)
	{
		return A.Index < B.Index;
	});
	return Info;
}

FTimelineUIInfo URecordManagerSubsystem::GetTimelineUIInfo(int Index)
{
	FTimelineUIInfo Info;
	if (GlobalTimeline.Timelines.Contains(Index))
	{
		const FEchoTimeline& Timeline = GlobalTimeline.Timelines[Index];
		Info.StartTimeKey = Timeline.StartTimeKey;
		Info.Length = Timeline.GetLastTimeKey();
		Info.Index = Index;
		Info.ActionKeys.Empty();
		Info.EchoColorStruct = RecordManagerSettings->EchoColors[Index];
		for (const FRecordActionKey& ActionKey : Timeline.ActionKeys)
		{
			FUIActionKey ActionInfo = FUIActionKey(ActionKey.TimeKey, ActionKey.Action.ActionEnum);
			Info.ActionKeys.Add(ActionInfo);
		}
	}
	return Info;
}

const float& URecordManagerSubsystem::GetCurrentTimeKey() const
{
	return CurrentTimeKey;
}

int URecordManagerSubsystem::GetSelectedTimelineSlot() const
{
	return SelectedSlot;
}

float URecordManagerSubsystem::GetGlobalTimelineLength()
{
	return GlobalTimeline.GetLength();
}

FTimelineUIInfo URecordManagerSubsystem::GetRecordingTimelineUIInfo()
{
	FTimelineUIInfo Info;
	if (!bIsRecording) return Info;
	
	Info.StartTimeKey = RecordingTimeline.StartTimeKey;
	Info.Length = RecordingTimeline.GetLastTimeKey();
	Info.Index = CurrentRecordingTimelineIndex;
	Info.ActionKeys.Empty();
	Info.EchoColorStruct = RecordManagerSettings->EchoColors[CurrentRecordingTimelineIndex];
	for (const FRecordActionKey& ActionKey : RecordingTimeline.ActionKeys)
	{
		FUIActionKey ActionInfo = FUIActionKey(ActionKey.TimeKey, ActionKey.Action.ActionEnum);
		Info.ActionKeys.Add(ActionInfo);
	}
	
	return Info;
}

void URecordManagerSubsystem::OnNewLevelLoaded(const TArray<AActor*>& Actors)
{
	for (AActor* Actor : Actors)
	{
		if (Actor->Implements<URecordableInterface>())
		{
			if (URecordableComponent* RecordableComponent = Actor->GetComponentByClass<URecordableComponent>(); RecordableComponent != nullptr)
			{
				UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::LevelStreaming, EEchoMessageType::Log, "Added Recordable to List : " + RecordableComponent->GetOwner()->GetName(), FColor::Magenta, 3.0f);
				RecordableComponents.Add(RecordableComponent);
				RecordableComponent->OnInteracted.AddDynamic(this, &URecordManagerSubsystem::OnRecordableInteractedWith);
			}
		}
		if (Actor->Implements<URecordListener>())
		{
			UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::LevelStreaming, EEchoMessageType::Log, "Added RecordListener to List : " + Actor->GetName(), FColor::Magenta, 3.0f);
			RecordListeners.Add(Actor);
		}
	}
}

void URecordManagerSubsystem::OnLevelUnloaded(const TArray<AActor*>& Actors)
{
	//Removing Record Listeners from Unloaded Level
	RecordListeners.RemoveAll([Actors](const AActor* RecordListener)
	{
		return Actors.Contains(RecordListener);
	});
	
	//Removing Recordable Components from Unloaded Level
	for (URecordableComponent* Recordable : RecordableComponents)
	{
		if (IsValid(Recordable) && Actors.Contains(Recordable->GetOwner()))
		{
			Recordable->StopRecording(true);
			Recordable->OnInteracted.RemoveDynamic(this, &URecordManagerSubsystem::OnRecordableInteractedWith);
		}
	}
	
	RecordableComponents.RemoveAll([Actors](const URecordableComponent* RecordableComponent)
	{
		return !IsValid(RecordableComponent) || Actors.Contains(RecordableComponent->GetOwner());
	});
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
	
	//Handle Player Rewinding
	if (bIsPlayerRewinding)
	{
		if (RecordingTimeline.StartTimeKey >= CurrentTimeKey)
		{
			StopPlayerRewind();
			return;
		}
		PlayPlayerRewind(previousTimeKey - RecordingTimeline.StartTimeKey, CurrentTimeKey - RecordingTimeline.StartTimeKey);
	}
	
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
					RecordableComponent->StopRewind(CurrentTimeKey);
					RecordableComponent->ReplayFirstKey();
					if (CurrentTimeKey < RecordableComponent->GetFirstInteractedKey()) RecordableComponent->StopRecording();
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
	if (!GlobalTimeline.Timelines.IsEmpty())
	{
		bool bHasReachedEnd = false;
		GlobalTimeline.Play(previousTimeKey, CurrentTimeKey, bIsInRewind, bHasReachedEnd);
		
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
		RecordingTimeline.RecordAnimationKey(RecordedActor, LocalTimeKey);
		
		if (LocalTimeKey >= RecordManagerSettings->MaxRecordTime)
		{
			//Reached MaxRecordTime, Stop Record
			StopRecord();
			UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Reached Max Record Time", FColor::Turquoise, 3.f);
		}
	}
	
	UIUpdateClock += DeltaTime;
	if (UIUpdateClock >= RecordManagerSettings->UIRefreshFrequency)
	{
		OnTimelineReplayUpdate.Broadcast(previousTimeKey, CurrentTimeKey, bIsRecording);
		UIUpdateClock -= RecordManagerSettings->UIRefreshFrequency;
	}
}

void URecordManagerSubsystem::PlayPlayerRewind(const float& PreviousTimeKey, const float& TimeKey)
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
	
	if (RecordedActor->GetClass()->ImplementsInterface(URecordHandlerInterface::StaticClass()))
	{
		TArray<FRecordActionKey> RewindActions;
		RecordingTimeline.GetActionKeys(PreviousTimeKey, TimeKey, true, RewindActions);
		for (const FRecordActionKey& RewindAction : RewindActions)
		{
			IRecordHandlerInterface::Execute_HandleRewindActionKey(RecordedActor, RewindAction.Action);
		}
	}
}

void URecordManagerSubsystem::OnEchoDestroyed(int EchoIndex)
{
	DestroyTimeline(EchoIndex);
}

void URecordManagerSubsystem::DestroySelectedTimeline()
{
	DestroyTimeline(SelectedSlot);
}

void URecordManagerSubsystem::DestroyTimeline(int TimelineIndex)
{
	if (bIsInRewind) return; //Forbid Timeline Destruction during Rewind
	if (!GlobalTimeline.Timelines.Contains(SelectedSlot)) return;
	GlobalTimeline.DestroyTimeline(SelectedSlot, EchoActorsPool);
	int DestroyedSlot = TimelineIndex;
	
	if (TimelineIndex == SelectedSlot)
	{
		//Decrement Until Correct Timeline
		DecrementSelectedSlot();
	}
	
	for (TObjectPtr<URecordableComponent> RecordableComponent : RecordableComponents)
	{
		if (!IsValid(RecordableComponent)) continue;
		RecordableComponent->HandleTimelineDestruction(DestroyedSlot);
	}
	
	if (GlobalTimeline.Timelines.IsEmpty())
	{
		CurrentTimeKey = 0.0f;
	}
	
	OnTimelineDestroyed.Broadcast(DestroyedSlot);
}

void URecordManagerSubsystem::IncrementSelectedSlot()
{
	int PreviouslySelectedSlot = SelectedSlot;
	if (GlobalTimeline.Timelines.IsEmpty()) SelectedSlot = 0;
	else
	{
		//Increment Until Correct Timeline is Selected
		do
		{
			SelectedSlot++;
			if (SelectedSlot >= GlobalTimeline.NbSlots) SelectedSlot = 0;
		}
		while (!GlobalTimeline.Timelines.Contains(SelectedSlot));
	}
	OnTimelineSelected.Broadcast(PreviouslySelectedSlot, SelectedSlot);
}

void URecordManagerSubsystem::DecrementSelectedSlot()
{
	int PreviouslySelectedSlot = SelectedSlot;
	
	if (GlobalTimeline.Timelines.IsEmpty()) SelectedSlot = 0;
	else
	{
		//Decrement Until Correct Timeline is Selected
		do 
		{
			SelectedSlot--;
			if (SelectedSlot < 0) SelectedSlot = GlobalTimeline.NbSlots - 1;
		}
		while (!GlobalTimeline.Timelines.Contains(SelectedSlot));
	}
	
	OnTimelineSelected.Broadcast(PreviouslySelectedSlot, SelectedSlot);
}

void URecordManagerSubsystem::SelectSlot(int Index, bool bCanSelectNonExistentTimeline)
{
	if (GlobalTimeline.Timelines.Contains(Index) || bCanSelectNonExistentTimeline)
	{
		int PreviouslySelectedSlot = SelectedSlot;
		SelectedSlot = Index;
		
		OnTimelineSelected.Broadcast(PreviouslySelectedSlot, SelectedSlot);
	}
}

void URecordManagerSubsystem::OnRecordableInteractedWith(URecordableComponent* Self, bool bShouldRecord, int RecordTimelineIndex)
{
	if (bShouldRecord)
	{
		if (!Self->IsRecording())
		{
			UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Starting to Record " + Self->GetOwner()->GetName(), FColor::Turquoise, 2.f);
			Self->StartRecording(FRecordInteractionKey(CurrentTimeKey, RecordTimelineIndex));
			Self->RecordKey(CurrentTimeKey);
		}
		else
		{
			UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Register Interaction Key for " + Self->GetOwner()->GetName() + " With Timeline Index : " + FString::FromInt(RecordTimelineIndex), FColor::Turquoise, 2.f);
			Self->RegisterInteractionKey(FRecordInteractionKey(CurrentTimeKey, RecordTimelineIndex));
		}
	}
	else
	{
		UEchoDebug::AddOnScreenDebugMessage(EEchoSystem::Record, EEchoMessageType::Log, "Stopping to Record " + Self->GetOwner()->GetName(), FColor::Turquoise, 2.f);
		Self->StopRecording(true);
	}
}
