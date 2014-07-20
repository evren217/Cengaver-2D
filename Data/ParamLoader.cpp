#include "../Utilities/Macro.h"
#include "../Utilities/Globals.h"
#include "ParamLoader.h"
#include "IniReader.h"
#include <stdio.h>

ParamLoader* ParamLoader::internalParamLoader = 0;

ParamLoader* ParamLoader::Instance()
{
	if (ParamLoader::internalParamLoader == 0)
	{
		ParamLoader::internalParamLoader = new ParamLoader();
	}

	return ParamLoader::internalParamLoader;
}

ParamLoader::ParamLoader()
{
}

ParamLoader::~ParamLoader(void)
{
	CLEAN(internalParamLoader);
}

void ParamLoader::LoadParamsFromFile(char* filename)
{
	CIniReader iniReader(filename);

	char section[8];

	sprintf_s(section, "params");

	TILESPROPERTIES = iniReader.ReadString(section, "TILESPROPERTIES", "");
	
	FRICTION_MAX = iniReader.ReadInteger(section, "FRICTION_MAX", 8);
	GRAVITY = iniReader.ReadInteger(section, "GRAVITY", 2);

	HERO_MAX_HEALTH = iniReader.ReadInteger(section, "HERO_MAX_HEALTH", 100);

	PARALAXSPEED1 = iniReader.ReadInteger(section, "PARALAXSPEED1", 20);
	PARALAXSPEED2 = iniReader.ReadInteger(section, "PARALAXSPEED2", 50);
	ANIMATIONSPEED = iniReader.ReadInteger(section, "ANIMATIONSPEED", 40);

	CURRENTTIME = (unsigned long)iniReader.ReadInteger(section, "CURRENTTIME", 300);
}