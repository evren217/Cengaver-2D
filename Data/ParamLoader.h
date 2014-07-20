#pragma once

class ParamLoader
{
public:
	// Singleton Pattern:
	static ParamLoader* Instance();
	virtual ~ParamLoader();

	void LoadParamsFromFile(char* filename);

private:
	ParamLoader();

	static ParamLoader* internalParamLoader;
};