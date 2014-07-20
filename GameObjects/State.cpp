#include "State.h"

State::State(void)
{
	name = "\0";
}

State::~State(void)
{
}

char* State::GetName(void){
	return name;
}