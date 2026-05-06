// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StandaloneEchoesHandler.generated.h"

struct FRecordedAction;
class AEchoActor;
class URecordManagerSettings;
struct FEchoTimeline;

UCLASS()
class PROJECTECHO_API AStandaloneEchoesHandler : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStandaloneEchoesHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Init Standalone Echoes Handler
	virtual void Init();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//Play CurrentFrame for all Active Timelines, activate timelines that have not yet been activated
	UFUNCTION()
	void Play(const float& PreviousTimeKey, const float& TimeKey, bool bIsInRewind, bool& bOutHasReachedEnd);
	
	//Play In Editor Current Key (Will Only play Transform Keys)
	UFUNCTION(BlueprintCallable)
	void PlayInEditor(const float& TimeKey);
	
	UFUNCTION(BlueprintCallable)
	void CreateTimelineFromEcho(AEchoActor* EchoActor);
	
	UFUNCTION(BlueprintCallable)
	int GetTimelineIndexFromEcho(AEchoActor* EchoActor);
	
	UFUNCTION(BlueprintCallable)
	int CreateTransformKey(int TimelineIndex, const float& TimeKey);
	
	UFUNCTION(BlueprintCallable)
	int CreateActionKey(int TimelineIndex, const float& TimeKey, const FRecordedAction& RecordedAction);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTimelinesLength();
	
	//Tool Setter Functions
	
	UFUNCTION(BlueprintCallable)
	void SetStartTimeKey(int TimelineIndex, float StartTimeKey);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEchoTimeline> EchoTimelines;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float CurrentTimeKey = 0.0f;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsRewind = false;
	
	UPROPERTY(BlueprintReadOnly)
	float RewindSpeed = 0.f;
	
	UPROPERTY()
	TObjectPtr<URecordManagerSettings> RecordManagerSettings = nullptr;
	
};
