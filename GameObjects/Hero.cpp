#include "Hero.h"
#include "State.h"
#include "../Data/TileManager.h"
#include "../Data/TileProperties.h"
#include "../Utilities/Globals.h"
#include "../Utilities/Macro.h"
#include "../Utilities/CollisionPoint.hpp"

using namespace std;

CHero::CHero(HDC* pBuffDC_p, short max_speed, short mass,char* heroBitmapPath) : CMovableObject(max_speed, mass)
{
	// Initialize the hero:
	movementVector = CVector2D(12*TILESIZE,92*TILESIZE);
	animationCounter=0;

	// Set Device Contextes:
	pBuffDC = pBuffDC_p;
	heroDeviceContext = CreateCompatibleDC(*pBuffDC);

	// Load Bitmaps:
	heroBitmap = LoadImage(NULL, heroBitmapPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(heroDeviceContext, heroBitmap);

	// Set Starting State:
	currentState = new Waiting();

	inventory = new Inventory(pBuffDC_p, "Resources/Inventory.bmp","Resources/SelectedGadget.bmp",this);

	health = HERO_MAX_HEALTH;
	maxKeyMovementSpeed = 4;
	useFriction = true;

	ai = new AI();
}

CHero::~CHero(void)
{
	DeleteDC(heroDeviceContext);
	DeleteObject(heroBitmap);
	CLEAN(currentState);
	CLEAN(inventory);
	CLEAN(ai);
}

void CHero::Draw(Viewpoint window)
{
	// Select tile property:
	tp = TileManager::Instance()->GetTileProperties(currentState->GetTile());

	// Transparant blit on deviceContext:
	TransparentBlt(*pBuffDC,
					X()-window.viewX ,
					Y()-window.viewY ,
					tp.bitmapWidth , 
					tp.bitmapHeight, 
					heroDeviceContext, 
					(tp.bitmapXPos+(animationCounter/ANIMATIONRATE))*tp.bitmapWidth, 
					tp.bitmapYPos*tp.bitmapHeight, 
					tp.bitmapWidth, 
					tp.bitmapHeight, 
					RGB(186,254,202));

	if(1==debugmode) {
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255,255,0));
		SelectObject(*pBuffDC, hPen);
		
		// State information:
		TextOut(*pBuffDC, (NewX()-window.viewX)-22,(NewY()-window.viewY)-42, currentState->GetName(), strlen(currentState->GetName()));	
		
		POINT pntArray[2];
		pntArray[0].x= X() - window.viewX;
		pntArray[0].y= Y() - window.viewY;
		pntArray[1].x= NewX() - window.viewX;
		pntArray[1].y= NewY() - window.viewY;

		Polyline(*pBuffDC, pntArray, 2);
		
		DeleteObject(hPen);
	}

	// Set animation counter:
	animationCounter++;
	animationCounter = animationCounter % (tp.bitmapCount*ANIMATIONRATE);
}

void CHero::SetCurrentState(State* state){
	CLEAN(currentState);
	currentState = static_cast<HeroState*>(state);
}
void CHero::MoveLeft(){
	if(movementVector.DX() > -maxKeyMovementSpeed)
		CMovableObject::MoveLeft();
}

void CHero::MoveRight(){
	if(movementVector.DX() < maxKeyMovementSpeed)
		CMovableObject::MoveRight();
}

void CHero::Jump(){
	CMovableObject::Jump();
	currentState->PressSpacebar(this);
}

void CHero::Move(long xMod, long yMod, bool absMod)
{
	CMovableObject::Move(xMod, yMod, absMod);

	ai->CheckCollisionWithEnemies(this);

	if(movementVector.NewX() == movementVector.X() && movementVector.NewY() == movementVector.Y())
		SetCurrentState(new Waiting());
	if(movementVector.NewX() == movementVector.X())
		currentState->StopMoving(this);
	if(movementVector.DX() > 0)
		currentState->PressRightArrow(this);
	if(movementVector.DX() < 0)
		currentState->PressLeftArrow(this);
}

void CHero::SelectGadget(unsigned char index)
{
	inventory->SetSelectedIndex(index); 
}

void CHero::DeleteSelectedGadget()
{
	inventory->RemoveGadget();
}

void CHero::IncreaseHealth(char value)
{
	if(health + value < HERO_MAX_HEALTH)
		health += value;
	else
		health = HERO_MAX_HEALTH;
}

void CHero::TakeHit(char hitAmount)
{
	health = health - hitAmount;

	if (IsDead()) currentState->TouchEnemy(this);
}

bool CHero::IsDead()
{
	if(CHero::health <= 0)
		return true;
	return false;
}

char CHero::GetHealth()
{
	return health;
}

void CHero::SetHealth(char h)
{
	CHero::health = h;
}