// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "EchoCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UEchoCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void ToggleSystemDebug();
};
