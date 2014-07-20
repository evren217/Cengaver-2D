#include "../Utilities/Macro.h"
#include "../Utilities/Globals.h"
#include "TileManager.h"
#include "TileProperties.h"
#include "TilePropertiesLoader.h"

TileManager* TileManager::internalTileManager = 0;

TileManager::TileManager()
{
	FillTileProperties();
}

TileManager::~TileManager()
{
	CLEAN(internalTileManager);
}

TileManager* TileManager::Instance()
{
	if(TileManager::internalTileManager == 0)
		TileManager::internalTileManager = new TileManager();
	return TileManager::internalTileManager;
}

TileProperties TileManager::GetTileProperties(unsigned char tileID) const
{
	return tilePropertyArray[tileID];
}

bool TileManager::IsTilePropertyInterestingForCollisionDetection(const unsigned char & tileID) const
{
	return IsTilePropertyInterestingForCollisionDetection(tilePropertyArray[tileID]);
}

bool TileManager::IsTilePropertyInterestingForCollisionDetection(const TileProperties & tp) const
{
	if(tp.id == 0)
		return false;

	if(tp.healthMod != 0)
		return true;

	if(tp.xMod != 0)
		return true;

	if(tp.yMod != 0)
		return true;

	if(tp.solid)
		return true;

	return false;
}

void TileManager::FillTileProperties()
{
	for (int i = 0; i < 255; i++)
	{
		tilePropertyArray[i].bitmapFile = "Resources/BackgroundTiles.bmp";
		tilePropertyArray[i].bitmapHeight = 16;
		tilePropertyArray[i].bitmapWidth = 16;
		tilePropertyArray[i].bitmapCount = 1;
		tilePropertyArray[i].bitmapXPos = 0;
		tilePropertyArray[i].bitmapYPos = 0;
		tilePropertyArray[i].id = i;
		tilePropertyArray[i].animation = false;
		tilePropertyArray[i].solid = false;
	}
	
	TilePropertiesLoader::Instance()->LoadPropertiesFromFile(TILESPROPERTIES, tilePropertyArray);
}

void TileManager::FillTileProperties(char* filename)
{
	TilePropertiesLoader::Instance()->LoadPropertiesFromFile(filename, tilePropertyArray);
}