// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Command.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType, Blueprintable)
struct FCommandContext
{
	GENERATED_BODY()
	
public:
	FCommandContext() = default;
	FCommandContext(AActor* InInstigator);
	FCommandContext(AActor* InInstigator, UWorld* InWorld);
	~FCommandContext() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> Instigator = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWorld> World = nullptr;
};


UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class PROJECTECHO_API UCommand : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	virtual void Execute();
	
	UFUNCTION(BlueprintCallable)
	virtual void ExecuteWithContext(const FCommandContext& Context);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveExecute();
};
