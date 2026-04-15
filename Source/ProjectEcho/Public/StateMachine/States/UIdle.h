#pragma once
#include "StateMachine/UState.h"
#include "UIdle.generated.h"

UCLASS()
class UIdle : public UState
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Enter() override;
	virtual void Exit() override;
private:

	virtual void OnMovePressed(FVector2D InMoveInput) override;
};
