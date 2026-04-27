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
	UMove();
	void Enter() override;
	void Exit() override;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MoveSpeed = 700;
	
};
