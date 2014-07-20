#pragma once

#include <windows.h>
#include "../GameObjects/Gadget.h"

struct
{
	unsigned char	id;

	// Bitmap properties:
	char*			bitmapFile;
	unsigned char	bitmapXPos;
	unsigned char	bitmapYPos;
	unsigned char	bitmapWidth;
	unsigned char	bitmapHeight;
	unsigned char	bitmapCount;
	bool			animation;
	bool			variation;
	char			layer;

	// Game Properties:
	bool			solid;
	char			xMod;
	char			yMod;
	bool			absMod;
	GadgetType		gadgetType;
	char			healthMod;
} typedef TileProperties;