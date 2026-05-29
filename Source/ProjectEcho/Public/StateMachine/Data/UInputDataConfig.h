#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UInputDataConfig.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class UInputDataConfig : public UDataAsset
{
public:
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* AMove;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* ARun;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* AJump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* ALook;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* AGrab;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* AThrowOrReflect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* ARecord;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* AIncrementSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* ADecrementSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* ADestroySlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* AInteract;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* APropulse;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* APause;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mappings")
	UInputMappingContext* IMCGlobal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mappings")
	UInputMappingContext* IMCStateMachine;
};