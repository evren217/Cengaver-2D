#pragma once
#include <Windows.h>
#include "TileProperties.h"
#include "TilePropertiesLoader.h"

class TileManager
{
public:
	// Singleton Pattern:
	static TileManager* Instance();
	virtual ~TileManager();

	TileProperties GetTileProperties(unsigned char tileID) const;
	bool IsTilePropertyInterestingForCollisionDetection(const unsigned char & tileID) const;
	bool IsTilePropertyInterestingForCollisionDetection(const TileProperties & tp) const;


	void FillTileProperties();
	void FillTileProperties(char* filename);

private:
	TileManager();
	static TileManager* internalTileManager;

	void LoadTiles();
	
	unsigned char tpIndex;
	TileProperties tilePropertyArray[255];
};