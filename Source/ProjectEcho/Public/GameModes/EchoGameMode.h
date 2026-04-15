// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EchoGameMode.generated.h"

class AEchoHUD;
class ACharacterST;
class APlayerStart;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTECHO_API AEchoGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void InitializeGame();
	void StartGame();
	void EndGame();

	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="LevelName"))
	void LoadStreamLevel(const FName& LevelName);
protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Initialize Game")
	void ReceiveInitializeGame();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive Start Game")
	void ReceiveStartGame();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Receive End Game")
	void ReceiveEndGame();

	UFUNCTION()
	void OnStreamLevelLoaded(int32 Linkage);
	
	UFUNCTION()
	void OnStreamLevelUnloaded(int32 Linkage);
	
	UFUNCTION()
	void OnPlayerDeathEnd();
	
	UPROPERTY()
	TObjectPtr<APlayerStart> EchoPlayerStart;
	UPROPERTY()
	TObjectPtr<ACharacterST> EchoPlayerCharacter;
	UPROPERTY()
	TObjectPtr<AEchoHUD> EchoHUD;

	UPROPERTY()
	TArray<FName> StreamLevelNames;
};
