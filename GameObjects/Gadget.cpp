#include <Windows.h>
#include "Gadget.h"
#include "../Data/TileProperties.h"
#include "../Data/TileManager.h"
#include "../UserInterface/Viewpoint.h"
#include "../Utilities/Macro.h"
#include "../Utilities/Globals.h"

Gadget::Gadget()
{
}

Gadget::Gadget(HDC* pBuffDC_p,int xPos_p,int yPos_p,bool inInventory_p,unsigned char tpID_p)
{
	amount = 1;
	
	// Save pointer to double buffer device context:
	pBuffDC = pBuffDC_p;
	
	// Set inventory:
	inInventory = inInventory_p;
	
	// Positions:
	xPos = xPos_p;
	yPos = yPos_p;
	tpID = tpID_p;

	// Get gadget bitmap:
	gadgetBMP = LoadImage(NULL, TileManager::Instance()->GetTileProperties(tpID_p).bitmapFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	// Create gadget device context:
	gadgetDeviceContext = CreateCompatibleDC(NULL);
	
	// Join the bitmap and the device context:
	SelectObject(gadgetDeviceContext,gadgetBMP);
	
	// Gadget animation counter:
	animationCounter = 0;
	generalAnimationIndent = ANIMATIONRATE;

	// Gadget type:
	type = TileManager::Instance()->GetTileProperties(tpID_p).gadgetType;
}

Gadget::~Gadget(void)
{
	DeleteDC(gadgetDeviceContext);
	DeleteObject(gadgetBMP);
}

void Gadget::Draw(Viewpoint window)
{
	if((xPos > window.viewX-TILESIZE && xPos < window.viewX+window.width+TILESIZE) && 
		(yPos > window.viewY-TILESIZE && yPos < window.viewY+window.height+TILESIZE) || inInventory)
	{
		// Get tile property:
		TileProperties tp = TileManager::Instance()->GetTileProperties(tpID);
		
		// Transparant blit on device context:
		TransparentBlt(*pBuffDC,
					xPos-window.viewX ,
					yPos-window.viewY ,
					tp.bitmapWidth , 
					tp.bitmapHeight, 
					gadgetDeviceContext, 
					(tp.bitmapXPos+animationCounter)*tp.bitmapWidth, 
					tp.bitmapYPos*tp.bitmapHeight, 
					tp.bitmapWidth, 
					tp.bitmapHeight, 
					TRANSPARENCYCOLOR);

		// Set animation counter:
		RaiseAnimationCounter();
	}
}

void Gadget::RaiseAnimationCounter()
{
	if(generalAnimationIndent==ANIMATIONRATE)
	{
		if(animationCounter<TileManager::Instance()->GetTileProperties(tpID).bitmapCount-1)
			animationCounter++;
		else
			animationCounter = 0;
		generalAnimationIndent = 0;
	}
	else
	{
		generalAnimationIndent++;
	}
}

void Gadget::DecreaseAmount(unsigned char v)
{
	if (v > amount)
		amount = 0;
	else
		amount -= v;
}