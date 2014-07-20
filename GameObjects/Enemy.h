#pragma once

#pragma comment( lib, "Msimg32.lib" )

#include <Windows.h>
#include "MovableObject.h"
#include "EnemyState.h"
#include "State.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IStateMachine.h"
#include "../Utilities/Vector2D.hpp"
#include "../UserInterface/Viewpoint.h"

class Enemy : public IDrawable, public CMovableObject, public IStateMachine
{
	class EnemyState* currentState;

public:
	Enemy(HDC* pBuffDC_p,short max_speed, short mass,char* enemyBitmapPath, unsigned char tileID_p, CVector2D mVector);
	virtual ~Enemy(void);

	void Draw(Viewpoint window);
	void Move(char xMod, char yMod,bool absMod);
	void SetCurrentState(State *state);
	void SetStartingState();
	void TakeHit(char hitAmount);
	bool TakeBossHit();

	unsigned char tileID;

private:
	HDC enemyDeviceContext;
	HDC* pBuffDC;
	HANDLE enemyBitmap;
	unsigned char animationCounter;
	unsigned char health;
};