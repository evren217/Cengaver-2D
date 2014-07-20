#pragma once

class State
{
public:
	State(void);
	~State(void);

	char* GetName(void);

protected:
	char* name;
};