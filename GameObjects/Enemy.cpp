#include "Enemy.h"
#include "State.h"
#include "../Utilities/Macro.h"
#include "../Data/TileManager.h"
#include "../Data/TileProperties.h"
#include "../Utilities/Globals.h"
#include "MovableObject.h"

Enemy::Enemy(HDC* pBuffDC_p,short max_speed, short mass, char* enemyBitmapPath, unsigned char tileID_p, CVector2D mVector) : CMovableObject(max_speed, mass)
{
	movementVector = mVector;

	animationCounter=0;

	// Set device contexts:
	pBuffDC = pBuffDC_p;
	enemyDeviceContext = CreateCompatibleDC(*pBuffDC);

	// Load enemy bitmap:
	enemyBitmap = LoadImage(NULL, enemyBitmapPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(enemyDeviceContext,enemyBitmap); 

	// Set the starting state:
	currentState = new WaitingEnemy();
	SetStartingState();

	// Set others:
	tileID = tileID_p;
	int p =0;
	useFriction = false;

	health = 1;
}

Enemy::~Enemy(void)
{
	DeleteDC(enemyDeviceContext);
	DeleteObject(enemyBitmap);
}

void Enemy::Draw(Viewpoint window)
{
	if((movementVector.X() > window.viewX-100 && movementVector.X() < window.width + window.viewX+100) &&
		(movementVector.Y() > window.viewY-100 && movementVector.Y() < window.height + window.viewY+100)){
		currentState->EnterViewpoint(this);
	}
	else{
		currentState->ExitViewpoint(this);
	}

	tp = TileManager::Instance()->GetTileProperties(tileID);

	TransparentBlt(*pBuffDC,
					X()-window.viewX,
					Y()-window.viewY,
					tp.bitmapWidth, 
					tp.bitmapHeight, 
					enemyDeviceContext, 
					(tp.bitmapXPos+(animationCounter/ANIMATIONRATE))*tp.bitmapWidth, 
					tp.bitmapYPos*TILESIZE*2,
					tp.bitmapWidth, 
					tp.bitmapHeight, 
					RGB(255,255,255));

	if(1==debugmode)
	{
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
		SelectObject(*pBuffDC, hPen);
		int newX = NewX();
		int newY = NewY();;
		Rectangle(*pBuffDC, (newX-window.viewX)-2, (newY-window.viewY)-2, (newX-window.viewX)+2, (newY-window.viewY)+2); 

		Rectangle(*pBuffDC, tp.bitmapXPos+newX, tp.bitmapYPos+newY, tp.bitmapXPos+tp.bitmapWidth+newX, tp.bitmapYPos+tp.bitmapHeight+newY); 

		TextOut(*pBuffDC, (newX-window.viewX)-22, (newY-window.viewY)-42, currentState->GetName(), strlen(currentState->GetName()));	
		DeleteObject(hPen);	
	}

	// Set animation counter:
	animationCounter++;
	animationCounter = animationCounter % (tp.bitmapCount*ANIMATIONRATE);
	
}

void Enemy::SetCurrentState(State* state){
	currentState->Exit(this);
	CLEAN(currentState);
	currentState = static_cast<EnemyState*>(state);
}

void Enemy::SetStartingState(){
	switch (tileID){
		// Weak Enemy:
		case 30:
			health = 10;
			SetCurrentState(new PatrollingEnemy());
			break;
		// Agile Enemy:
		case 31:
			health = 10;
			SetCurrentState(new HoppingEnemy());
			break;
		// Strong Enemy:
		case 32:
			health = 15;
			SetCurrentState(new RammingEnemy());
			break;
		// Boss Enemy:
		case 33:
			health = 15;
			SetCurrentState(new BossEnemy());
			break;
	}
}

void Enemy::Move(char xMod, char yMod,bool absMod){
	CMovableObject::Move(xMod, yMod, absMod);
	currentState->Execute(this);
}

void Enemy::TakeHit(char hitAmount)
{
	health--;

	if (health <= 0)
	{
		SetCurrentState(new DyingEnemy());
	}
}

bool Enemy::TakeBossHit()
{
	health--;

	if (health <= 0)
	{
		SetCurrentState(new DyingEnemy());
 		return true;
	}

	return false;
}