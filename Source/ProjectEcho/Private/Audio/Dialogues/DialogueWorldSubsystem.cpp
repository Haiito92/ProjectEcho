// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/Dialogues/DialogueWorldSubsystem.h"
#include "DataAssetDeveloperSettings.h"
#include "EchoSystem.h"
#include "Audio/Dialogues/DialogueSystemSettings.h"
#include "Commands/CommandFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/Debug/EchoDebug.h"
#include "Tools/Debug/EchoMessageType.h"

void UDialogueWorldSubsystem::InitializeDialogueSubsystem()
{
	const UDataAssetDeveloperSettings* DataDevSettings = GetDefault<UDataAssetDeveloperSettings>();
	
	if (!IsValid(DataDevSettings)) return;
	
	SystemSettings = DataDevSettings->DialogueSettings.LoadSynchronous();
}

void UDialogueWorldSubsystem::QueueDialogue2D(const FQueuedDialogueInfo& QueuedDialogueInfo)
{
	if (!IsValid(QueuedDialogueInfo.DialogueQueued))
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Audio, EEchoMessageType::Error, "Can't queue dialogue: dialogue is not valid!", FColor::Red, 3.0f);
		return;
	}
	
	if (CurrentlyPlayedDialogueInfo.DialogueQueued == nullptr)
	{
		PlayDialogue2D(QueuedDialogueInfo);
		return;
	}
	
	QueuedDialogues.Add(QueuedDialogueInfo);
}

void UDialogueWorldSubsystem::PlayNextDialogue2D()
{
	if (QueuedDialogues.IsEmpty())
	{
		UEchoDebug::LogAndAddOnScreenDebugMessage(EEchoSystem::Audio, EEchoMessageType::Error, "Can't play next dialogue: queue is empty!", FColor::Red, 3.0f);
		return;
	}
	
	FQueuedDialogueInfo NextDialogueInfo = QueuedDialogues[0];
	QueuedDialogues.RemoveAt(0);
	
	PlayDialogue2D(NextDialogueInfo);
}

void UDialogueWorldSubsystem::PlayDialogue2D(const FQueuedDialogueInfo& DialogueToPlayInfo)
{
	if (!IsValid(DialogueToPlayInfo.DialogueQueued)) return;
	
	float DialogueDuration = DialogueToPlayInfo.DialogueQueued->GetDuration();
	
	FTimerDelegate DialogueTimerDelegate;
	DialogueTimerDelegate.BindUObject(this, &ThisClass::OnCurrentDialogueEnded);
	
	GetWorld()->GetTimerManager().SetTimer(
		PlayedDialogueTimerHandle,
		DialogueTimerDelegate,
		DialogueDuration,
		false);
	
	CurrentlyPlayedDialogueInfo = DialogueToPlayInfo;
	
	UGameplayStatics::PlaySound2D(GetWorld(), DialogueToPlayInfo.DialogueQueued);
}

void UDialogueWorldSubsystem::OnCurrentDialogueEnded()
{
	FDialogueCommandData* GlobalCommandData = SystemSettings->DialogueCommands.Find(CurrentlyPlayedDialogueInfo.DialogueQueued);
	
	if (GlobalCommandData != nullptr)
	{
		if (!GlobalCommandData->bReachEndOnce)
		{
			GlobalCommandData->bReachEndOnce = true;
			UCommandFunctionLibrary::ExecuteCommandsWithContextFromWorld(GlobalCommandData->OnceDialogueEndCommands, GetWorld());
		}
		
		UCommandFunctionLibrary::ExecuteCommandsWithContextFromWorld(GlobalCommandData->DialogueEndCommands, GetWorld());
	}
	
	UCommandFunctionLibrary::ExecuteCommandsWithContextFromWorld(CurrentlyPlayedDialogueInfo.DialogueEndCommands, GetWorld());
	
	
	CurrentlyPlayedDialogueInfo = FQueuedDialogueInfo();
	
	PlayNextDialogue2D();
}
