#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataConfig.generated.h"

class UInputAction;

UCLASS()
class UInputDataConfig : public UDataAsset
{
public:
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AMove;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ARun;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AJump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ALook;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AGrab;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AThrow;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ARegister;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AIncrementSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ADecrementSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ADestroySlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* APlaySlot;
};