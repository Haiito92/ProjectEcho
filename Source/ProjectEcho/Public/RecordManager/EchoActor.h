// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EchoColorStruct.h"

#include "EchoActor.generated.h"

struct FRecordedAction;
class IRecordableInterface;
enum class ERecordedAction : uint8;

UCLASS()
class PROJECTECHO_API AEchoActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEchoActor();
	
public:
	void HandleActionKey(const FRecordedAction& Action);
	
	UFUNCTION()
	void SetControlRotation(const FRotator& ControlRotation);
	
	UFUNCTION()
	void InitEcho(const int& index, const FEchoColorStruct& EchoColor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveInitEcho(FEchoColorStruct EchoColor);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveHandleActionKey(ERecordedAction Action);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveSetControlRotation(const FRotator& ControlRotation);
	
	UFUNCTION(BlueprintCallable)
	void RegisterRecordable(TScriptInterface<IRecordableInterface> Recordable);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegisterRecordable, TScriptInterface<IRecordableInterface>, Recordable);
	UPROPERTY(BlueprintAssignable)
	FOnRegisterRecordable OnRegisterRecordable;
	
	UPROPERTY(BlueprintReadOnly)
	int EchoIndex = 0;
};
