#pragma once

#include "../Gadget.h"
#include "../Hero.h"
#include "../../Utilities/Globals.h"

class Health : public Gadget
{
public:
	Health(HDC* pBuffDC_p, int xPos_p, int yPos_p, bool inInventory_p, unsigned char tpID_p) : Gadget(pBuffDC_p, xPos_p, yPos_p, inInventory_p, tpID_p)
	{
		amount = 10;
	}

	GadgetInstantiationEquipment Use(CHero* owner)
	{
		if (amount > 0 && owner->GetHealth() != HERO_MAX_HEALTH)
		{
			owner->IncreaseHealth(1);
			DecreaseAmount(1);
		}

		return GadgetInstantiationEquipment::GIE_NONE;
	}
};