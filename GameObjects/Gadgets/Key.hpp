#pragma once

#include "../Gadget.h"

class Key : public Gadget
{
public:	
	Key(HDC* pBuffDC_p, int xPos_p, int yPos_p, bool inInventory_p, unsigned char tpID_p) : Gadget(pBuffDC_p, xPos_p, yPos_p, inInventory_p, tpID_p)
	{
	}
	GadgetInstantiationEquipment Use(CHero* object)
	{
		return GadgetInstantiationEquipment::GIE_NONE;
	}
};