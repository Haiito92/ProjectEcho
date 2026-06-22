// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEvents/GameEventInfo.h"
#include "GameFramework/GameModeBase.h"
#include "EchoGameModeBase.generated.h"

class UGameEventSubsystem;
class UMenuEventHolder;
class AEchoPlayerControllerBase;
class AEchoHUDBase;
/**
 * 
 */
UCLASS()
class PROJECTECHO_API AEchoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void SpawnActors();
	virtual void InitializeGame();
	virtual void InitializeUI();
	
private:
	void StartGame();
	UFUNCTION(BlueprintCallable)
	void EndGame();
	
public:
	UFUNCTION(BlueprintCallable)
	void ToggleGamePause();
	UFUNCTION(BlueprintCallable)
	virtual void PauseGame();
	UFUNCTION(BlueprintCallable)
	virtual void ResumeGame();
protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Spawn Actors")
	void ReceiveSpawnActors();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Initialize Game")
	void ReceiveInitializeGame();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Initialize UI")
	void ReceiveInitializeUI();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Start Game")
	void ReceiveStartGame();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive End Game")
	void ReceiveEndGame();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Pause Game")
	void ReceivePauseGame();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Resume Game")
	void ReceiveResumeGame();
	
	UFUNCTION()
	void OnPauseInputStarted();
	
	UFUNCTION()
	void OnGameEventLaunched(const FGameEventInfo& EventInfo);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMenuEventHolder> MenuEventHolder;
	
	UPROPERTY()
	TObjectPtr<AEchoHUDBase> EchoHUD;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AEchoPlayerControllerBase> EchoPlayerController;
	
	UPROPERTY()
	bool bIsGamePaused = false;
	
	UPROPERTY()
	TObjectPtr<UGameEventSubsystem> GameEventSubsystem;
};
