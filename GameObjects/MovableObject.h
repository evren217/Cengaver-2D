#pragma once

#include "AI.h"
#include "../Utilities/Vector2D.hpp"
#include "../Utilities/CollisionPoint.hpp"
#include "../Data/TileProperties.h"

class Level;

class CMovableObject
{
public:
	CMovableObject(short max_speed, short mass);
	virtual ~CMovableObject(void);

	virtual void MoveLeft();
	virtual void MoveRight();
	virtual void Jump();
	virtual void Stop();
	virtual void TakeHit(char hitAmount);

	int X() const { return movementVector.X(); }
	int Y() const{ return movementVector.Y(); }
	int NewX() const { return movementVector.NewX(); }
	int NewY() const{ return movementVector.NewY(); }
	int Width() const { return tp.bitmapWidth; }
	int Height() const{ return tp.bitmapHeight; }
	short max_speed;

	void SetAI(AI* ai_P);
	void Move(long xMod, long yMod, bool absMod);
	void SetAILevel(Level* level_p);
	CVector2D movementVector;
	CollisionPointGroup prevCollsions;
	AI* ai;

protected:
	TileProperties tp;
	bool useFriction;

private:
	double acceleration;
	short mass;
	int jumping;
	unsigned short frictionCounter;
};