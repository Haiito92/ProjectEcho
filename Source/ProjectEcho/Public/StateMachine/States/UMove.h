// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine/UState.h"
#include "UMove.generated.h"

UCLASS()
class PROJECTECHO_API UMove : public UState
{
	GENERATED_BODY()
public:
	void Enter() override;
	virtual void Tick(float DeltaTime) override;
	void Exit() override;
	virtual void InitState(UStateMachine* InStateMachine, ACharacterST* InCharacter) override;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MoveSpeed = 700;
	
};
