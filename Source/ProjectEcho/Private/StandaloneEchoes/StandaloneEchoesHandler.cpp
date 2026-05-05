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

void AStandaloneEchoesHandler::CreateTimelineFromEcho(AEchoActor* EchoActor)
{
	if (EchoActor == nullptr) return;
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

