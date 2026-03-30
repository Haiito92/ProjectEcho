#pragma once
#include <string>
#include <unordered_map>

#include "State.h"

class StateMachine : UActorComponent
{
private:
	State* PtrCurrentState;
	State* PtrPreviousState;
	
	std::unordered_map<std::string, State*> StateMap;
public:
	
	virtual void InitStates() = 0;
	void StartState(const std::string& newState);
	void AddState(State* newState,const std::string& stateName);
	void ChangeState(const std::string& stateName);
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
