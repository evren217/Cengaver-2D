#pragma once

#include <Windows.h>
#include "Interfaces/IDrawable.h"
#include "../UserInterface/Viewpoint.h"
#include "../Utilities/Globals.h"
#include "Gadget.h"

class Inventory : public IDrawable
{
public:
	Inventory(HDC* pBuffDC_p, char* inventoryBitmapPath, char* selectedGadgetbackBitmapPath, CHero* owner_p);
	virtual ~Inventory(void);

	char GetGadgetCount(void);
	bool AddGadget(Gadget* gadget);
	void RemoveGadget(unsigned char index);
	void RemoveGadget();
	Gadget* GetSelectedGadget();
	void Draw(Viewpoint window);
	void SetSelectedIndex(unsigned char index);
	GadgetInstantiationEquipment UseGadget();
	bool FindAndRemoveKey();

private:
	Gadget* gadgets[INVENTORY_SIZE];
	unsigned char gadgetCount;
	unsigned char selectedIndex;

	HDC inventoryDeviceContext;
	HDC selectedGadgetBackgroundDC;
	HDC* pBuffDC;
	HANDLE inventoryBitmap;
	HANDLE selectedBackgroundBitmap;
	HFONT displayFont;

	CHero* owner;

	int SetSameTypeOnIndexAndRemove(int index);
};