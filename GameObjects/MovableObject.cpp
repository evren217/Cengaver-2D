#include "MovableObject.h"
#include "AI.h"
#include "Level.h"
#include "../Utilities/Macro.h"
#include "../Utilities/Globals.h"

CMovableObject::CMovableObject(short p_max_speed, short p_mass)
{
	max_speed = p_max_speed;
	mass = p_mass;
	acceleration = 0.2;
	ai = new AI();
	jumping = 0;
	frictionCounter = FRICTION_MAX;
}

CMovableObject::~CMovableObject(void)
{
	CLEAN(ai);
}

void CMovableObject::MoveLeft()
{
	movementVector.IncreaseDX(-1);
}

void CMovableObject::MoveRight()
{
	movementVector.IncreaseDX(+1);
}

void CMovableObject::Jump()
{
	if(jumping == 0)
	{
		movementVector.IncreaseDY(-20);
		jumping = 20;
	}
}

void CMovableObject::Move(long xMod, long yMod, bool absMod)
{
	// Gravity:
	movementVector.IncreaseDY(GRAVITY);

	// Friction:
	if(useFriction)
	{
		if(frictionCounter > 0)
		{
			frictionCounter--;	
		}
		else
		{	
			if(movementVector.DX() > 0)
				movementVector.IncreaseDX(-1);
			if(movementVector.DX() < 0)
				movementVector.IncreaseDX(1);
			frictionCounter = FRICTION_MAX;
		}
	}
	
	// Modifiers:
	if(absMod)
	{
		movementVector.SetDX(0);
		movementVector.SetDY(0);
	}

	movementVector.IncreaseDX(xMod);
	movementVector.IncreaseDY(yMod);
	
	if(jumping > 0)
	{
		jumping -= 1;
	}
	
	if(movementVector.DX() > TILESIZE-1)
		movementVector.SetDX(TILESIZE-2);
	
	else if(movementVector.DX() < -(TILESIZE-1))
		movementVector.SetDX(-(TILESIZE-2));

	if(movementVector.X() < 1*TILESIZE)
	{
		movementVector.SetX(1*TILESIZE);
		movementVector.SetDX(0);
	}
	else if(movementVector.X() > LEVEL_MAX_WIDTH*TILESIZE)
	{
		movementVector.SetX(LEVEL_MAX_WIDTH*TILESIZE-TILESIZE);
		movementVector.SetDX(0);
	}
	if(movementVector.Y() < TILESIZE)
	{
		movementVector.SetY(TILESIZE);
		movementVector.SetDY(0);
	}
	else if(movementVector.Y() > (LEVEL_MAX_HEIGHT-1)*TILESIZE)
	{
		movementVector.SetY((LEVEL_MAX_HEIGHT-1)*TILESIZE-TILESIZE);
		movementVector.SetDY(0);
	}

	prevCollsions = ai->CheckCollision(this);
	
	movementVector.Step();
}

void CMovableObject::Stop()
{
	movementVector.SetNewX(movementVector.X());
}

void CMovableObject::SetAI(AI *ai_P)
{
	ai = ai_P;
}

void CMovableObject::SetAILevel(Level *level_p)
{
	if(ai)
	{
		ai->SetLevel(level_p);
	}
}

void CMovableObject::TakeHit(char hitAmount)
{
}