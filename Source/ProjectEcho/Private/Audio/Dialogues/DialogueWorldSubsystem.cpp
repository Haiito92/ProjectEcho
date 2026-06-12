// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/Dialogues/DialogueWorldSubsystem.h"

#include "EchoSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void UDialogueWorldSubsystem::InitializeDialogueSubsystem()
{
}

void UDialogueWorldSubsystem::QueueDialogue2D(USoundBase* DialogueToQueue)
{
	if (!IsValid(DialogueToQueue))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Audio, EEchoMessageType::Error, "Can't queue dialogue: dialogue is not valid!", FColor::Red, 3.0f);
		return;
	}
	
	if (CurrentlyPlayedDialogue == nullptr)
	{
		PlayDialogue2D(DialogueToQueue);
		return;
	}
	
	QueuedDialogues.Add(DialogueToQueue);
}

void UDialogueWorldSubsystem::PlayNextDialogue2D()
{
	if (QueuedDialogues.IsEmpty())
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Audio, EEchoMessageType::Error, "Can't play next dialogue: queue is empty!", FColor::Red, 3.0f);
		return;
	}
	
	USoundBase* NextDialogue = QueuedDialogues[0];
	QueuedDialogues.RemoveAt(0);
	
	PlayDialogue2D(NextDialogue);
}

void UDialogueWorldSubsystem::PlayDialogue2D(USoundBase* DialogueToPlay)
{
	float DialogueDuration = DialogueToPlay->GetDuration();
	
	FTimerDelegate DialogueTimerDelegate;
	DialogueTimerDelegate.BindUObject(this, &ThisClass::OnCurrentDialogueEnded);
	
	GetWorld()->GetTimerManager().SetTimer(
		PlayedDialogueTimerHandle,
		DialogueTimerDelegate,
		DialogueDuration,
		false);
	
	UGameplayStatics::PlaySound2D(GetWorld(), DialogueToPlay);
}

void UDialogueWorldSubsystem::OnCurrentDialogueEnded()
{
	CurrentlyPlayedDialogue = nullptr;
	
	PlayNextDialogue2D();
}
