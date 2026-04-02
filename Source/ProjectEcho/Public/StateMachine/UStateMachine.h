#pragma once
#include "Public/StateMachine/UState.h"
#include "UStateMachine.generated.h"

UCLASS()
class UStateMachine : public UActorComponent
{
	GENERATED_BODY()
public:
	UStateMachine();
	virtual void InitStates();
	void StartState(const FString& newState);
	void AddState(UState* newState,const FString& nameState);
	void ChangeState(const FString& newState);
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UState* CurrentState;
	UPROPERTY()
	UState* PreviousState;

	UPROPERTY()
	TMap<FString,UState*> StateMap;
	
	UPROPERTY()
	ACharacter* Owner;
};
