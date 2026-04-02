#pragma once
#include "StateMachine/UState.h"
#include "UWalk.generated.h"

UCLASS()
class UWalk : public UState
{
public:
	GENERATED_BODY()
	virtual void Tick(float DeltaTime) override;

	
};
