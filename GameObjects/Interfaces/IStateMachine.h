#pragma once

#include "State.h"

class State;

class IStateMachine
{
public:
	virtual void SetCurrentState(State* state)=0;
};