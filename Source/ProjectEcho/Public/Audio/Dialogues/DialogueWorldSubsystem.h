// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UDialogueWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void InitializeDialogueSubsystem();
	
	UFUNCTION(BlueprintCallable)
	void QueueDialogue2D(USoundBase* DialogueToQueue);
	
private:
	
	void PlayNextDialogue2D();
	void PlayDialogue2D(USoundBase* DialogueToPlay);
	void OnCurrentDialogueEnded();
	
	
	UPROPERTY()
	USoundBase* CurrentlyPlayedDialogue;
	UPROPERTY()
	FTimerHandle PlayedDialogueTimerHandle;
	
	UPROPERTY()
	TArray<USoundBase*> QueuedDialogues;
};
