#include "../Utilities/Macro.h"
#include "TilePropertiesLoader.h"
#include "TileProperties.h"
#include "IniReader.h"
#include <stdio.h>

TilePropertiesLoader* TilePropertiesLoader::internalTilePropertiesLoader = 0;

TilePropertiesLoader* TilePropertiesLoader::Instance()
{
	if (TilePropertiesLoader::internalTilePropertiesLoader == 0)
	{
		TilePropertiesLoader::internalTilePropertiesLoader = new TilePropertiesLoader();
	}

	return TilePropertiesLoader::internalTilePropertiesLoader;
}

TilePropertiesLoader::TilePropertiesLoader()
{
}

TilePropertiesLoader::~TilePropertiesLoader(void)
{
	CLEAN(internalTilePropertiesLoader);
}

void TilePropertiesLoader::LoadPropertiesFromFile(char* filename, TileProperties tpArray[])
{
	CIniReader iniReader(filename);

	unsigned char prefAmount = iniReader.SectionCount();

	for (unsigned char i = 0; i < prefAmount; i++)
	{
		char section[8];
		sprintf_s(section, "pref%d", i);

		unsigned char id = (unsigned char)iniReader.ReadInteger(section, "id", i);

		tpArray[id].id				= id;
		tpArray[id].bitmapFile		= iniReader.ReadString(section, "bitmapFile", "");
		tpArray[id].bitmapXPos		= (unsigned char)iniReader.ReadInteger(section, "bitmapXPos", 0);
		tpArray[id].bitmapYPos		= (unsigned char)iniReader.ReadInteger(section, "bitmapYPos", 0);
		tpArray[id].bitmapWidth		= (unsigned char)iniReader.ReadInteger(section, "bitmapWidth", 0);
		tpArray[id].bitmapHeight	= (unsigned char)iniReader.ReadInteger(section, "bitmapHeight", 0);
		tpArray[id].bitmapCount		= (unsigned char)iniReader.ReadInteger(section, "bitmapCount", 0);
		tpArray[id].animation		= iniReader.ReadBoolean(section, "animation", false);
		tpArray[id].variation		= iniReader.ReadBoolean(section, "variation", false);
		tpArray[id].layer			= (char)iniReader.ReadInteger(section, "layer", -1);
		tpArray[id].solid			= iniReader.ReadBoolean(section, "solid", false);
		tpArray[id].gadgetType      = (GadgetType)iniReader.ReadInteger(section, "gadgetType", 0);
		tpArray[id].xMod			= (char)iniReader.ReadInteger(section,"xMod",0);
		tpArray[id].yMod			= (char)iniReader.ReadInteger(section,"yMod",0);
		tpArray[id].absMod			= iniReader.ReadBoolean(section, "absMod", false);
		tpArray[id].healthMod		= (char)iniReader.ReadInteger(section, "healthMod", 0);
	}
}

void TilePropertiesLoader::LoadProperties(TileProperties tp)
{
}