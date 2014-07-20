#pragma once

#pragma comment( lib, "Msimg32.lib" )

#include <Windows.h>
#include "MovableObject.h"
#include "HeroState.h"
#include "State.h"
#include "Inventory.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IStateMachine.h"
#include "../Utilities/Vector2D.hpp"
#include "../UserInterface/Viewpoint.h"

class HeroState;

class CHero : public IDrawable, public CMovableObject, public IStateMachine
{
	class HeroState* currentState;

public:
	CHero(HDC* pBuffDC_p,short max_speed, short mass,char* heroBitmapPath);
	virtual ~CHero(void);

	void MoveLeft();
	void MoveRight();
	void Jump();
	void Move(long xMod, long yMod, bool absMod);
	void SelectGadget(unsigned char index);
	void DeleteSelectedGadget();
	bool IsDead();
	void TakeHit(char hitAmount);

	void Draw(Viewpoint window);
	void SetCurrentState(State *state);

	void IncreaseHealth(char value);
	char GetHealth();
	void SetHealth(char h);

	Inventory* inventory;

protected:
	HDC heroDeviceContext;
	HDC* pBuffDC;
	HANDLE heroBitmap;
	unsigned char animationCounter;
	char health;
	unsigned char maxKeyMovementSpeed;
};