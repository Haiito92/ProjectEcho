// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueWorldSubsystem.generated.h"

class UDialogueSystemSettings;
class USoundBase;
class UCommand;
/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct FQueuedDialogueInfo
{
	GENERATED_BODY()
	
public:
	FQueuedDialogueInfo() = default;
	~FQueuedDialogueInfo() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> DialogueQueued = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCommand*> DialogueEndCommands = TArray<UCommand*>();
};

UCLASS()
class PROJECTECHO_API UDialogueWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void InitializeDialogueSubsystem();
	
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="QueuedDialogueInfo"))
	void QueueDialogue2D(const FQueuedDialogueInfo& QueuedDialogueInfo);
	
private:
	
	void PlayNextDialogue2D();
	void PlayDialogue2D(const FQueuedDialogueInfo& DialogueToPlayInfo);
	void OnCurrentDialogueEnded();
	
	
	UPROPERTY()
	FQueuedDialogueInfo CurrentlyPlayedDialogueInfo;
	UPROPERTY()
	FTimerHandle PlayedDialogueTimerHandle;
	
	UPROPERTY()
	TArray<FQueuedDialogueInfo> QueuedDialogues;
	
	UPROPERTY()
	TObjectPtr<UDialogueSystemSettings> SystemSettings;
};
