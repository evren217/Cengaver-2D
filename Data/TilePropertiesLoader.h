#pragma once

#include "TileProperties.h"

class TilePropertiesLoader
{
public:
	// Singleton Pattern:
	static TilePropertiesLoader* Instance();
	virtual ~TilePropertiesLoader();

	void LoadPropertiesFromFile(char* filename, TileProperties tpArray[]);
	void LoadProperties(TileProperties tp);

private:
	TilePropertiesLoader();

	static TilePropertiesLoader* internalTilePropertiesLoader;
};