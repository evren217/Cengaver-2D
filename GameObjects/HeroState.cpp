#include "HeroState.h"
#include "State.h"
#include "../Utilities/Macro.h"
#include "../Utilities/Globals.h"

// ---------------------------------------------------
//	class: HeroState
// ---------------------------------------------------
HeroState::HeroState(void)
{
	name = "None\0";
	tile = HS_NONE;
}

HeroState::~HeroState(void)
{
}

unsigned char HeroState::GetTile(void){
	return tile;
}

void HeroState::TouchEnemy(CHero *h){
	h->SetCurrentState(new Dead());
}

void HeroState::StepOnExit(CHero *h)
{
	h->SetCurrentState(new Finished());
}


void HeroState::SelectGadget(CHero* h)
{
	h->SetCurrentState(new SelectingGadget());
}

void HeroState::PressRightArrow(CHero *h){}
void HeroState::PressLeftArrow(CHero *h){}
void HeroState::PressSpacebar(CHero *h){}
void HeroState::StopMoving(CHero *h){}

// ---------------------------------------------------
//	class: Waiting
// ---------------------------------------------------
Waiting::Waiting(void)
{
	name = "Waiting\0";
	tile = HS_WAITING;
}
void Waiting::PressLeftArrow(CHero* h)
{
	h->SetCurrentState(new MoveLeft());
}

void Waiting::PressRightArrow(CHero* h)
{
	h->SetCurrentState(new MoveRight());
}

void Waiting::PressSpacebar(CHero* h)
{
	h->SetCurrentState(new Jumping());
}

// ---------------------------------------------------
//	class: MoveRight
// ---------------------------------------------------
MoveRight::MoveRight(void)
{
	name = "MoveRight\0";
	tile = HS_MOVERIGHT;
}

void MoveRight::PressLeftArrow(CHero* h)
{
	h->SetCurrentState(new MoveLeft());
}

void MoveRight::StopMoving(CHero* h)
{
	h->SetCurrentState(new Waiting());
}

// ---------------------------------------------------
//	class: MoveLeft
// ---------------------------------------------------
MoveLeft::MoveLeft(void)
{
	name = "MoveLeft\0";
	tile = HS_MOVELEFT;
}

void MoveLeft::PressRightArrow(CHero* h)
{
	h->SetCurrentState(new MoveRight());
}

void MoveLeft::StopMoving(CHero* h)
{
	h->SetCurrentState(new Waiting());
}

// ---------------------------------------------------
//	class: Jumping
// ---------------------------------------------------
Jumping::Jumping(void)
{
	name = "Jumping\0";
}

void Jumping::StopMoving(CHero* h)
{
	h->SetCurrentState(new Waiting());
}

// ---------------------------------------------------
//	class: Finished
// ---------------------------------------------------
Finished::Finished(void)
{
	name = "Finished\0";
}

// ---------------------------------------------------
//	class: SelectGadget
// ---------------------------------------------------
SelectGadget::SelectGadget(void)
{
	name = "SelectGadget\0";
}

// ---------------------------------------------------
//	class: Dead
// ---------------------------------------------------
Dead::Dead(void)
{
	name = "Dead\0";
	tile = HS_DEAD;
}
void Dead::DieSequence(CHero* h)
{
}

// ---------------------------------------------------
//	class: SelectingGadget
// ---------------------------------------------------
SelectingGadget::SelectingGadget(void)
{
	name = "UseGadget\0";
	tile = HS_USEGADGET;
}