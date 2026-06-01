// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "EchoViewport.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTECHO_API UEchoViewport : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	void InitializeViewport();
	
	virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;
	
	virtual bool InputAxis(const FInputKeyEventArgs& Args) override;
};
