#include <Windows.h>
#include "Game.h"
#include "Gadget.h"
#include "../Utilities/Macro.h"
#include "../Utilities/Globals.h"
#include "../Utilities/Vector2D.hpp"
#include "../UserInterface/Viewpoint.h"
#include "../Data/TileManager.h"
#include "Gadgets/Health.hpp"
#include "Gadgets/Star.hpp"
#include "string.h"
#include "../Data/GameLoader.h"
#include "../UserInterface/CharacterStatusBar.h"

CGame::CGame(HDC* pBuffDC_p)
{
	// Reset parameters:
	running = 1;
	currentLevel = 1;
	level = 0;
	score = 0;
	gadgetSelector = 0;
	keyHandler = 0;

	// Create the hero(cengaver):
	hero = new CHero(pBuffDC_p, 5, 5, "Resources/cengaver.bmp");

	// Create character status bar:
	statusBar = new CharacterStatusBar(pBuffDC_p);

	// Load level:
	pBuffDC = pBuffDC_p;

	NextLevel();

	window.width = 576;
	window.height = 320;
	window.viewX = hero->X()-(window.width/2); 
	window.viewY = hero->Y()-(window.height/2);

	currentTime = CURRENTTIME*40;

	// Set display colors for text: 
	SetBkMode(*pBuffDC, TRANSPARENT);
	SetTextColor(*pBuffDC,RGB(255,255,255));
}

CGame::~CGame(void)
{
	CLEAN(level);
	CLEAN(hero);
	CLEAN(statusBar);
}

unsigned char CGame::GetRunning()
{
	return running;
}

char CGame::Update()
{
	// Save the current position of the hero:
	int prevX = hero->X();
	int prevY = hero->Y();

	// Handle key inputs:
	HandleKeys();

	// Move the hero:
	POINT mods = level->CalculateTileModifiers(hero->prevCollsions);
	bool absMod = level->CalculateTileAbsMod(hero->prevCollsions);
	hero->Move(mods.x,mods.y,absMod);
	PickUpGadget();

	// Calculate the end of level:
	if(level->TouchEnd(hero->prevCollsions))
	{

		currentLevel++;
		if(NextLevel()==false)
		{
			EndGame();
			return GAME_END_END;
		}
		else
		{
			AdjustViewpoint(prevX,prevY);
			return GAME_END_LEVEL;
		}
	}

	// Check doors:
	POINT p = level->CheckDoor(hero->prevCollsions);
	if(p.x != -1)
	{
		if(hero->inventory->FindAndRemoveKey())
		{
			level->RemoveDoor(p.x,p.y,0,0);
		}
	}

	if(hero->IsDead())
	{
		//EndGame();
		return GAME_END_DEAD;
	}

	// Apply environment health modifiers:
	hero->IncreaseHealth(level->CalcEnvironementDamage(hero->prevCollsions));

	// Move enemies:
	level->worldRep.UpdateEnemies(window);
	
	// Change the paralax background positions:
	level->ChangeParalaxX(hero->movementVector,1);
	level->ChangeParalaxX(hero->movementVector,2);

	// Adjust the current viewpoint to the hero's position:
	AdjustViewpoint(prevX,prevY);

	// Update the timer:
	UpdateTimer();

	return -1;
}

void CGame::Draw()
{
	// Draw the background:
	level->DrawParalaxBackGrounds(window);

	// Draw the level background:
	level->Draw(window,-1);

	// Draw the gadgets:
	level->worldRep.DrawAllGadgetsInRepository(window);

	// Draw the enemies:
	level->worldRep.DrawAllEnemiesInRepository(window);

	// Draw the hero:
	hero->Draw(window);

	// Draw the level foreground:
	level->Draw(window,1);

	// Draw inventory:
	hero->inventory->Draw(window);

	// Draw character status bar:
	statusBar->Draw(hero->GetHealth(),currentTime, GetScore());
}

void CGame::AdjustViewpoint(int prevX,int prevY)
{
	int diffX = abs(hero->X()-prevX);
	int diffY = abs(hero->Y()-prevY);
	
	if(hero->X() > (window.viewX+window.width)-SCROLL_BORDERX)
		window.viewX+=diffX;
	if(hero->X() < window.viewX+SCROLL_BORDERX)
		window.viewX-=diffX;

	if(window.viewX < 0)
		window.viewX = 0;
	if(window.viewX > (LEVEL_MAX_WIDTH*TILESIZE)-window.width)
		window.viewX = (LEVEL_MAX_WIDTH*TILESIZE)-window.width;

	if(hero->Y() > (window.viewY+window.height)-SCROLL_BORDERY)
		window.viewY+=diffY;
	if(hero->Y() < window.viewY+SCROLL_BORDERY)
		window.viewY-=diffY;

	if(window.viewY < 0 )
		window.viewY = 0;
	if(window.viewY > (LEVEL_MAX_HEIGHT*TILESIZE)-window.height)
		window.viewY = (LEVEL_MAX_HEIGHT*TILESIZE)-window.height;
}

void CGame::PickUpGadget()
{
	Gadget* g = level->worldRep.GetGadgetAtPos(hero);

	if(g != 0 && hero->inventory->GetGadgetCount() != INVENTORY_SIZE)
	{
		// Increase score if collected Star, otherwise add into the inventory:
		if (g->tpID == 70)
		{
			level->worldRep.RemoveGadgetFromRepository(g);
			IncreaseScore(1);
		}	
		else
		{
			level->worldRep.RemoveGadgetFromRepository(g);
			hero->inventory->AddGadget(g);
		}
	}
}

void CGame::HandleKeys()
{
	if(GetFocus() != hWnd)
		return;

	// Hero Movements:
	if(GetKeyState(VK_LEFT) & 0x8000)
	{
		hero->MoveLeft();
	}
	if(GetKeyState(VK_RIGHT) & 0x8000)
	{
		hero->MoveRight();
	}
	if(GetKeyState(VK_DOWN) & 0x8000)
	{
		hero->Stop();
	}
	if(GetKeyState(' ') & 0x8000){
		hero->Jump();
	}

	// Debug Toggle:
	if(GetKeyState(VK_F5) & 0x8000){
		if(1==debugmode)
		{
			debugmode=0;
		}
		else
		{
			debugmode=1;
		}
	}

	// Select gadgets in inventory:
	if((GetKeyState('z') & 0x8000) || (GetKeyState('Z') & 0x8000)){
		if(GetKeyState('Z') != keyHandler) {
			keyHandler = GetKeyState('Z');

			if((--gadgetSelector) < 0)
				gadgetSelector = 7;
			hero->SelectGadget(gadgetSelector);
		}
	}
	if((GetKeyState('x') & 0x8000) || (GetKeyState('X') & 0x8000)){
		if(GetKeyState('X') != keyHandler) {
			keyHandler = GetKeyState('X');
		
			if((++gadgetSelector) > 7)
				gadgetSelector = 0;
			hero->SelectGadget(gadgetSelector);
		}
	}
	if(GetKeyState('1') & 0x8000){
		hero->SelectGadget(0);
		gadgetSelector = 0;
	}
	if(GetKeyState('2') & 0x8000){
		hero->SelectGadget(1);
		gadgetSelector = 1;
	}
	if(GetKeyState('3') & 0x8000){
		hero->SelectGadget(2);
		gadgetSelector = 2;
	}
	if(GetKeyState('4') & 0x8000){
		hero->SelectGadget(3);
		gadgetSelector = 3;
	}
	if(GetKeyState('5') & 0x8000){
		hero->SelectGadget(4);
		gadgetSelector = 4;
	}
	if(GetKeyState('6') & 0x8000){
		hero->SelectGadget(5);
		gadgetSelector = 5;
	}
	if(GetKeyState('7') & 0x8000){
		hero->SelectGadget(6);
		gadgetSelector = 6;
	}
	if(GetKeyState('8') & 0x8000){
		hero->SelectGadget(7);
		gadgetSelector = 7;
	}

	// Delete gadget:
	if(GetKeyState(VK_BACK) & 0x8000){
		hero->DeleteSelectedGadget();
	}

	// Exit game:
	if(GetKeyState(VK_ESCAPE) & 0x8000){
		running = 0;
	}

	// Use gadget:
	if((GetKeyState('e') & 0x8000) || (GetKeyState('E') & 0x8000)){
		if(GetKeyState('E') != keyHandler) {
			keyHandler = GetKeyState('E');
		
			hero->inventory->UseGadget();
		}
	}

	// Debug Mode:
	if(1==debugmode)
	{
		// Background Scrolling:
		if(GetKeyState(VK_NUMPAD5) & 0x8000){
			if(window.viewY-SCROLL_RESOLUTION>0)
				window.viewY-=SCROLL_RESOLUTION;
		}
		if(GetKeyState(VK_NUMPAD2) & 0x8000){
			if(window.viewY+SCROLL_RESOLUTION< (LEVEL_MAX_HEIGHT*TILESIZE)-window.height)
				window.viewY+=SCROLL_RESOLUTION;
		}
		if(GetKeyState(VK_NUMPAD1) & 0x8000){
			if(window.viewX-SCROLL_RESOLUTION>0)
				window.viewX-=SCROLL_RESOLUTION;
		}
		if(GetKeyState(VK_NUMPAD3) & 0x8000){
			if(window.viewX+SCROLL_RESOLUTION<(LEVEL_MAX_WIDTH*TILESIZE)-window.width)
				window.viewX+=SCROLL_RESOLUTION;
		}

		// Die:
		if((GetKeyState('d') & 0x8000) || (GetKeyState('D') & 0x8000))
		{
			hero->TakeHit(10);
		}

		if((GetKeyState('M') & 0x8000) || (GetKeyState('M') & 0x8000))
		{
			hero->IncreaseHealth(-100);
		}
	}
}

bool CGame::LoadLevel(unsigned char level_p)
{
	bool returnvalue = false;

	switch(level_p)
	{
		case 1:
			level = new Level(pBuffDC);	
			level->readLevel("Levels/level1.properties");
			returnvalue = true;
			break;
		case 2:
			level = new Level(pBuffDC);
			level->readLevel("Levels/level2.properties");
			returnvalue = true;
			break;
		case 3:
			level = new Level(pBuffDC);
			level->readLevel("Levels/level3.properties");
			returnvalue = true;
			break;
		case 4:
			level = new Level(pBuffDC);
			level->readLevel("Levels/level4.properties");
			returnvalue = true;
			break;
		default:
			returnvalue = false;
			break;
	}

	hero->SetAILevel(level);

	return returnvalue;
}

bool CGame::NextLevel()
{
	CLEAN(level);
	bool result = LoadLevel(currentLevel);
	if(result == true)
	{
		POINT p = level->FindBeginPoint();

		// Set hero parameters:
		hero->movementVector.SetX(p.x*TILESIZE);
		hero->movementVector.SetY(p.y*TILESIZE);
		hero->movementVector.SetDX(0);
		hero->movementVector.SetDY(0);

		AdjustViewpoint(hero->movementVector.X(),hero->movementVector.Y());

		return result;
	}
	else
	{
		return result;	
	}
}

void CGame::UpdateTimer()
{
	currentTime--;
	if (currentTime/40 == 0) {
		hero->IncreaseHealth(-100);
		currentTime = CURRENTTIME*40;
	}

	// Update key handler:
	if (currentTime%40 == 0)
		keyHandler = 0;
	if(keyHandler == GetKeyState('Z'))
		if(!(GetKeyState('Z') & 0x8000))
			keyHandler = 0;
	if(keyHandler == GetKeyState('X'))
		if(!(GetKeyState('X') & 0x8000))
			keyHandler = 0;
	if(keyHandler == GetKeyState('E'))
		if(!(GetKeyState('E') & 0x8000))
			keyHandler = 0;	
}

void CGame::EndGame()
{
	currentLevel = 0;
}

CHero* CGame::GetHero()
{
	return hero;
}

void CGame::IncreaseScore(long value)
{
	score += value;
}

long CGame::GetScore()
{
	return score;
}