// Fill out your copyright notice in the Description page of Project Settings.


#include "StandaloneEchoes/StandaloneEchoesHandler.h"

#include "DataAssetDeveloperSettings.h"
#include "RecordManager/EchoActor.h"
#include "RecordManager/RecordManagerSubsystem.h"


// Sets default values
AStandaloneEchoesHandler::AStandaloneEchoesHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStandaloneEchoesHandler::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
}

// Called every frame
void AStandaloneEchoesHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float PreviousTimeKey = CurrentTimeKey;
	CurrentTimeKey += DeltaTime * (bIsRewind ? -RewindSpeed : 1.f);
	CurrentTimeKey = FMath::Max(CurrentTimeKey, 0);
	
	bool bHasReachedEnd = false;
	Play(PreviousTimeKey, CurrentTimeKey, bIsRewind, bHasReachedEnd);
	
	if (bHasReachedEnd && !bIsRewind)
	{
		bIsRewind = true;
	}
	else if (bIsRewind && CurrentTimeKey == 0)
	{
		bIsRewind = false;
	}
}

void AStandaloneEchoesHandler::Init()
{
	RecordManagerSettings = GetDefault<UDataAssetDeveloperSettings>()->RecordManagerSettings.LoadSynchronous();

	RewindSpeed = GetTimelinesLength() / RecordManagerSettings->StandaloneGlobalRewindTime;
}

void AStandaloneEchoesHandler::Play(const float& PreviousTimeKey, const float& TimeKey, bool bIsInRewind,
                                    bool& bOutHasReachedEnd)
{
	bool bHasNotReachedEnd = false;

	for (FEchoTimeline& EchoTimeline : EchoTimelines)
	{
		if (EchoTimeline.EchoActor == nullptr) continue;
		
		// Activate or deactivate Timeline
		float LocalTimeKey = TimeKey - EchoTimeline.StartTimeKey;
		if (EchoTimeline.bIsActive)
		{
			if (bIsInRewind? LocalTimeKey < 0 : EchoTimeline.GetLastTimeKey() < LocalTimeKey)
			{
				//Timeline Desactivation (Reached End of Last Key // In Rewind, Reached passed FirstKey)
				EchoTimeline.ActivateTimeline(false);
				continue;
			}
		}
		else if (LocalTimeKey < EchoTimeline.GetLastTimeKey())
		{
			if (LocalTimeKey > 0)
			{
				//Timeline Activation (Reached FirstKey)
				EchoTimeline.ActivateTimeline(true);
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
		EchoTimeline.PlayReplay(PreviousTimeKey - EchoTimeline.StartTimeKey, LocalTimeKey, bIsInRewind);
	}
	
	bOutHasReachedEnd = bIsInRewind ? false : !bHasNotReachedEnd;
}

TMap<int /*TimelineIndex*/, FRotator /*ControlRotation*/> AStandaloneEchoesHandler::PlayInEditor(const float& GlobalTimeKey)
{
	TMap<int /*TimelineIndex*/, FRotator /*ControlRotation*/> ControlRotMap;
	for (int i = 0; i < EchoTimelines.Num(); ++i)
	{
		if (!EchoTimelines.IsValidIndex(i)) continue;
		if (EchoTimelines[i].EchoActor == nullptr) continue;
		
		EchoTimelines[i].PlayTransformKeys(GlobalTimeKey - EchoTimelines[i].StartTimeKey);
		ControlRotMap.Add(i, EchoTimelines[i].GetControlRotationOfCurrentKey(GlobalTimeKey));
	}
	return ControlRotMap;
}

void AStandaloneEchoesHandler::CreateTimelineFromEcho(AEchoActor* EchoActor)
{
	if (EchoActor == nullptr) return;
#if WITH_EDITOR
	this->Modify();
#endif
	FEchoTimeline EchoTimeline;
	EchoTimeline.EchoActor = EchoActor;
	EchoTimeline.StartTimeKey = CurrentTimeKey;
	EchoTimeline.RecordTransformKey(EchoActor, CurrentTimeKey);
	EchoTimeline.RecordTransformKey(EchoActor, CurrentTimeKey + 0.5f);
	EchoTimelines.Add(EchoTimeline);
}

int AStandaloneEchoesHandler::GetTimelineIndexFromEcho(AEchoActor* EchoActor)
{
	for (int i = 0; i < EchoTimelines.Num(); ++i)
	{
		if (EchoActor == EchoTimelines[i].EchoActor) return i;
	}
	return -1;
}

int AStandaloneEchoesHandler::CreateTransformKey(int TimelineIndex, const float& LocalTimeKey)
{
	if (TimelineIndex > EchoTimelines.Num() || TimelineIndex < 0) return -1;
#if WITH_EDITOR
	this->Modify();
#endif
	EchoTimelines[TimelineIndex].RecordTransformKey(EchoTimelines[TimelineIndex].EchoActor, LocalTimeKey);
	for (int i = 0; i < EchoTimelines[TimelineIndex].TransformKeys.Num(); ++i)
	{
		if (EchoTimelines[TimelineIndex].TransformKeys[i].TimeKey == LocalTimeKey) return i;
	}
	return -1;
}

void AStandaloneEchoesHandler::ReplaceTransformKey(int TimelineIndex, const float& LocalTimeKey, const FRotator& ControlRotation, bool bRecordIfNotFound)
{
	if (EchoTimelines.IsValidIndex(TimelineIndex))
	{
#if WITH_EDITOR
		this->Modify();
#endif
		EchoTimelines[TimelineIndex].ReplaceTransformKey(EchoTimelines[TimelineIndex].EchoActor, LocalTimeKey, bRecordIfNotFound, &ControlRotation);
	}
}

bool AStandaloneEchoesHandler::HasTransformKey(int TimelineIndex, const float& LocalTimeKey)
{
	if (EchoTimelines.IsValidIndex(TimelineIndex))
	{
		return EchoTimelines[TimelineIndex].HasTransformKey(LocalTimeKey);
	}
	return false;
}

int AStandaloneEchoesHandler::ModifyTransformKeyTimeKey(int TimelineIndex, const float& TimeKey,
	const float& NewTimeKey)
{
	if (EchoTimelines.IsValidIndex(TimelineIndex))
	{
		FRecordTransformKey* FoundTransformKey = EchoTimelines[TimelineIndex].TransformKeys.FindByPredicate([TimeKey](const FRecordTransformKey& TransformKey)
		{
			return TransformKey.TimeKey == TimeKey;
		});
		if (FoundTransformKey == nullptr) return -1;
#if WITH_EDITOR
		this->Modify();
#endif
		FoundTransformKey->TimeKey = NewTimeKey;
		EchoTimelines[TimelineIndex].TransformKeys.Sort([](const FRecordTransformKey& A, const FRecordTransformKey& B)
		{
			return A.TimeKey < B.TimeKey;
		});
		for (int i = 0; i < EchoTimelines[TimelineIndex].TransformKeys.Num(); ++i)
		{
			if (EchoTimelines[TimelineIndex].TransformKeys[i].TimeKey == NewTimeKey) return i;
		}
	}
	return -1;
}

int AStandaloneEchoesHandler::CreateActionKey(int TimelineIndex, const float& LocalTimeKey,
											  const FRecordedAction& RecordedAction)
{
	if (TimelineIndex > EchoTimelines.Num() || TimelineIndex < 0) return -1;
#if WITH_EDITOR
	this->Modify();
#endif
	EchoTimelines[TimelineIndex].ActionKeys.Add(FRecordActionKey(LocalTimeKey, RecordedAction));
	EchoTimelines[TimelineIndex].ActionKeys.Sort([](const FRecordActionKey& A, const FRecordActionKey& B)
	{
		return A.TimeKey < B.TimeKey;
	});
	
	for (int i = 0; i < EchoTimelines[TimelineIndex].ActionKeys.Num(); ++i)
	{
		if (EchoTimelines[TimelineIndex].ActionKeys[i].TimeKey == LocalTimeKey &&
			EchoTimelines[TimelineIndex].ActionKeys[i].Action == RecordedAction) return i;
	}
	return -1;
}

int AStandaloneEchoesHandler::ModifyActionKeyTimeKey(int TimelineIndex, const int& KeyIndex, const float& NewTimeKey)
{
	if (EchoTimelines.IsValidIndex(TimelineIndex))
	{
		if (!EchoTimelines[TimelineIndex].ActionKeys.IsValidIndex(KeyIndex)) return -1;
#if WITH_EDITOR
		this->Modify();
#endif
		EchoTimelines[TimelineIndex].ActionKeys[KeyIndex].TimeKey = NewTimeKey;
		const FRecordedAction Action = EchoTimelines[TimelineIndex].ActionKeys[KeyIndex].Action;
		EchoTimelines[TimelineIndex].ActionKeys.Sort([](const FRecordActionKey& A, const FRecordActionKey& B)
		{
			return A.TimeKey < B.TimeKey;
		});
		for (int i = 0; i < EchoTimelines[TimelineIndex].ActionKeys.Num(); ++i)
		{
			if (EchoTimelines[TimelineIndex].ActionKeys[i].TimeKey == NewTimeKey &&
				EchoTimelines[TimelineIndex].ActionKeys[i].Action == Action) return i;
		}
	}
	return -1;
}

void AStandaloneEchoesHandler::ModifyActionKeyAction(int TimelineIndex,  const int& KeyIndex,
	const FRecordedAction& RecordedAction)
{
	if (EchoTimelines.IsValidIndex(TimelineIndex))
	{
		if (!EchoTimelines[TimelineIndex].ActionKeys.IsValidIndex(KeyIndex)) return;
#if WITH_EDITOR
		this->Modify();
#endif
		EchoTimelines[TimelineIndex].ActionKeys[KeyIndex].Action = RecordedAction;
	}
}

void AStandaloneEchoesHandler::RecreateAllRewindActions(int TimelineIndex)
{
	if (EchoTimelines.IsValidIndex(TimelineIndex))
	{
#if WITH_EDITOR
		this->Modify();
#endif
		EchoTimelines[TimelineIndex].RewindActionKeys.Empty();
		for (const FRecordActionKey& RecordActionKey : EchoTimelines[TimelineIndex].ActionKeys)
		{
			if (RewindEquivalentActions.Contains(RecordActionKey.Action.ActionEnum))
			{
				FRecordActionKey RewindKey = FRecordActionKey();
				RewindKey.TimeKey = RecordActionKey.TimeKey;
				RewindKey.Action = FRecordedAction(RewindEquivalentActions[RecordActionKey.Action.ActionEnum]);
				EchoTimelines[TimelineIndex].RewindActionKeys.Add(RewindKey);
			}
		}
	}
}

float AStandaloneEchoesHandler::GetTimelinesLength()
{
	float LastTimeKey = 0;
	for (const FEchoTimeline& EchoTimeline : EchoTimelines)
	{
		if (LastTimeKey < EchoTimeline.GetLastTimeKey() + EchoTimeline.StartTimeKey)
		{
			LastTimeKey = EchoTimeline.GetLastTimeKey() + EchoTimeline.StartTimeKey;
		}
	}
	return LastTimeKey;
}

void AStandaloneEchoesHandler::SetStartTimeKey(int TimelineIndex, float StartTimeKey)
{
	if (EchoTimelines.IsValidIndex(TimelineIndex))
	{
#if WITH_EDITOR
		this->Modify();
#endif
		EchoTimelines[TimelineIndex].StartTimeKey = StartTimeKey;
	}
}

