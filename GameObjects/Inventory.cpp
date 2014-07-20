#include "Inventory.h"
#include "../Utilities/Globals.h"
#include "Gadget.h"
#include "../Utilities/Macro.h"

Inventory::Inventory(HDC* pBuffDC_p, char* inventoryBitmapPath,char* selectedGadgetbackBitmapPath, CHero* owner_p)
{
	// Load double buffering:
	pBuffDC = pBuffDC_p;
	
	// Create compatible device context:
	inventoryDeviceContext = CreateCompatibleDC(*pBuffDC);
	selectedGadgetBackgroundDC = CreateCompatibleDC(*pBuffDC);
	
	// Load inventory bitmap:
	inventoryBitmap = LoadImage(NULL, inventoryBitmapPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	selectedBackgroundBitmap = LoadImage(NULL, selectedGadgetbackBitmapPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	// Join bitmap and device context:
	SelectObject(inventoryDeviceContext,inventoryBitmap); 
	SelectObject(selectedGadgetBackgroundDC,selectedBackgroundBitmap);
	
	// Set gadget count:
	gadgetCount = 0;
	
	// Set selected index:
	selectedIndex = 0;
	
	// Reset gadget array:
	for(int i=0;i<INVENTORY_SIZE;i++)
		gadgets[i] = 0;
	
	// Create default font:
	displayFont = CreateFont(-9, 0, 0, 0, FW_DONTCARE, false, false, false, OEM_CHARSET,OUT_DEFAULT_PRECIS, 
							  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE, "Arial");

	owner = owner_p;
}

Inventory::~Inventory(void)
{
	DeleteDC(inventoryDeviceContext);
	DeleteDC(selectedGadgetBackgroundDC);
	DeleteObject(selectedBackgroundBitmap);
	DeleteObject(inventoryBitmap);
	DeleteObject(displayFont);

	for(int i=0;i<INVENTORY_SIZE;i++)
		CLEAN(gadgets[i]);
}

void Inventory::Draw(Viewpoint window){


	// Inventory Size:
	int imageWidth = 160;
	int imageHeight = 27;

	// Placement on window:
	int xOriginDest = (window.width/2) - (imageWidth/2) ;
	int yOriginDest = window.height - (imageHeight+25);

	// Placement in bitmap:
	int xSrcDest = 0;
	int ySrcDest = 0;

	
	// Transparant blit on device context:
	TransparentBlt(*pBuffDC,
					xOriginDest ,
					0 ,
					imageWidth , 
					imageHeight, 
					inventoryDeviceContext, 
					0, 
					0, 
					imageWidth, 
					imageHeight, 
					TRANSPARENCYCOLOR);


	// Draw all gadgets in the inventory:
	for(int i=0,xIndent = 5; i<INVENTORY_SIZE; i++,xIndent+=18)
	{
		if(i == 4)
		{
			xIndent+=6;
		}

		// Draw the gadget selector:
		if(selectedIndex == i)
			TransparentBlt(*pBuffDC, xOriginDest+xIndent, 7, 16, 16, selectedGadgetBackgroundDC,0, 0, 16, 16, TRANSPARENCYCOLOR);
		
		if(gadgets[i] != 0)
		{
			Gadget* g = gadgets[i];
			g->xPos = xOriginDest+xIndent+window.viewX;
			g->yPos = 6+window.viewY; 

			HGDIOBJ h = SelectObject(*pBuffDC,displayFont);
			char j[5];
			itoa(g->amount,j,10);
			int offset = 0;
			if(strlen(j)==1)
				offset = 6;
			else if(strlen(j)==2)
				offset = 3;
			else
				offset = 1;		

			// Draw gadget:
			g->Draw(window);

			// Draw text:
			TextOut(*pBuffDC,  xOriginDest+xIndent+offset, 25, j, strlen(j));
			SelectObject(*pBuffDC,h);
		}
	}
}

char Inventory::GetGadgetCount(void)
{
	return gadgetCount;
}

void Inventory::RemoveGadget(unsigned char index)
{
	if(!gadgets[index] == 0 && index < INVENTORY_SIZE)
	{
		CLEAN(gadgets[index]);
		gadgets[index] = 0;
		gadgetCount--;
	}
}

void Inventory::RemoveGadget()
{
	RemoveGadget(selectedIndex);
}

GadgetInstantiationEquipment Inventory::UseGadget()
{
	if(!gadgets[selectedIndex] == 0 && selectedIndex < INVENTORY_SIZE)
	{
		Gadget* g = gadgets[selectedIndex];

		GadgetInstantiationEquipment gie = g->Use(owner);

		if (g->amount == 0) 
			SetSameTypeOnIndexAndRemove(selectedIndex);

		return gie;
	}

	return GadgetInstantiationEquipment::GIE_NONE;
}

bool Inventory::AddGadget(Gadget* gadget)
{
	for(int i=0; i<INVENTORY_SIZE;i++)
	{
		if(gadgets[i] == 0)
		{
			gadgets[i] = gadget;
			gadget->inInventory = true;
			gadgetCount++;
			return true;
		}
	}
	return false;
}

void Inventory::SetSelectedIndex(unsigned char index)
{
	if(index > -1 && index < INVENTORY_SIZE)
		selectedIndex = index;
}

Gadget* Inventory::GetSelectedGadget()
{
	if(gadgets[selectedIndex] != 0)
		return gadgets[selectedIndex];
	return 0;
}

bool Inventory::FindAndRemoveKey()
{
	for(int i=0;i<INVENTORY_SIZE;i++)
	{
		if(gadgets[i] != 0 && gadgets[i]->tpID == 72)
		{
			RemoveGadget(i);
			return true;
		}
	}
	return false;
}

int Inventory::SetSameTypeOnIndexAndRemove(int index)
{
	for(int i=0;i<INVENTORY_SIZE;i++)
	{
		if(gadgets[i] != 0 && i!= index && gadgets[index]->tpID == gadgets[i]->tpID)
		{
			RemoveGadget(index);
			gadgets[index] = gadgets[i];
			gadgets[i] = 0;
			return i;
		}
	}
	RemoveGadget(index);
	return -1;
}