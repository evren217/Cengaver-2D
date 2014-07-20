#pragma once
#include <Windows.h>
#include "Interfaces/IDrawable.h"
#include "../UserInterface/Viewpoint.h"

class CHero;
class CMovableObject;

enum GadgetType { GT_NONE = 0, GT_HEALTH, GT_STAR, GT_KEY };
enum GadgetInstantiationEquipment { GIE_NONE };

class Gadget : public IDrawable
{
public:
	Gadget();
	Gadget(HDC* pBuffDC_p,int xPos_p,int yPos_p,bool inInventory_p,unsigned char tpID_p);
	~Gadget(void);
	
	void Draw(Viewpoint window);
	virtual GadgetInstantiationEquipment Use(CHero* owner)=0;
	
	bool inInventory;
	int	xPos;
	int	yPos;

	unsigned char amount;
	unsigned char tpID;

private:
	unsigned char animationCounter;
	unsigned char generalAnimationIndent;

	GadgetType type;


	HDC*	pBuffDC;
	HDC		gadgetDeviceContext;
	HANDLE	gadgetBMP;

	void RaiseAnimationCounter();

protected:
	void DecreaseAmount(unsigned char v);
};