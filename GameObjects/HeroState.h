#pragma once

#include "State.h"
#include "Hero.h"

class CHero;

class HeroState : public State
{
public:
	HeroState(void);
	~HeroState(void);

	virtual void TouchEnemy(CHero* h);
	virtual void PressRightArrow(CHero* h);
	virtual void PressLeftArrow(CHero* h);
	virtual void PressSpacebar(CHero* h);
	virtual void StopMoving(CHero* h);
	virtual void StepOnExit(CHero* h);
	virtual void SelectGadget(CHero* h);

	unsigned char GetTile(void);

protected:
	unsigned char tile;
};

class Waiting : public HeroState
{
public:
	Waiting(void);

	void Waiting::PressLeftArrow(CHero* h);
	void Waiting::PressRightArrow(CHero* h);
	void Waiting::PressSpacebar(CHero* h);
};

class MoveRight : public HeroState
{
public:
	MoveRight(void);
	
	void MoveRight::StopMoving(CHero* h);
	void MoveRight::PressLeftArrow(CHero* h);
};

class MoveLeft : public HeroState
{
public:
	MoveLeft(void);

	void MoveLeft::StopMoving(CHero* h);
	void MoveLeft::PressRightArrow(CHero* h);
};

class Jumping : public HeroState
{
public:
	Jumping(void);

	void Jumping::StopMoving(CHero* h);
};

class SelectGadget : public HeroState
{
public:
	SelectGadget(void);
};

class Finished : public HeroState
{
public:
	Finished(void);
};

class Dead : public HeroState 
{
public:
	Dead(void);
	void Dead::DieSequence(CHero* hero);
};

class SelectingGadget : public HeroState 
{
public:
	SelectingGadget(void);
};