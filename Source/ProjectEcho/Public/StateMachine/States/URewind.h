#pragma once
#include "StateMachine/UState.h"
#include "URewind.generated.h"

UCLASS()
class URewind : public UState
{
	GENERATED_BODY()
public:
	URewind();
	
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
};
