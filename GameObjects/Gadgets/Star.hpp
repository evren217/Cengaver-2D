#pragma once

#include "../Gadget.h"

class Star : public Gadget
{
public:

	Star(HDC* pBuffDC_p, int xPos_p, int yPos_p, bool inInventory_p, unsigned char tpID_p) : Gadget(pBuffDC_p, xPos_p, yPos_p, inInventory_p, tpID_p)
	{
	}

	GadgetInstantiationEquipment Use(CHero* owner)
	{
		if (amount > 0)
		{
			DecreaseAmount(1);
		}

		return GadgetInstantiationEquipment::GIE_NONE;
	}
};