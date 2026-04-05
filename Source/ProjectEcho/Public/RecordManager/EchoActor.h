// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecordableInterface.h"

#include "EchoActor.generated.h"

enum class ERecordedAction : uint8;

UCLASS()
class PROJECTECHO_API AEchoActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEchoActor();
	
public:
	UFUNCTION()
	void HandleActionKey(ERecordedAction Action);
	
	UFUNCTION()
	void SetControlRotation(const FRotator& ControlRotation);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveHandleActionKey(ERecordedAction Action);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveSetControlRotation(const FRotator& ControlRotation);
	
};
