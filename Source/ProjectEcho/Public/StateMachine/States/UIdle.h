#pragma once
#include "StateMachine/UState.h"
#include "UIdle.generated.h"

UCLASS()
class UIdle : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	
};
