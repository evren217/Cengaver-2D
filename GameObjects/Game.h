#pragma once

#include <Windows.h>
#include <list>
#include "Hero.h"
#include "Level.h"
#include "Enemy.h"
#include "Gadget.h"
#include "../UserInterface/CharacterStatusBar.h"

class CGame
{
public:
	CGame(HDC* pBuffDC_p);
	virtual ~CGame(void);

	char Update();
	void Draw();
	void DrawTime();
	bool NextLevel();
	CHero* GetHero();
	bool LoadLevel(unsigned char level);
	
	bool running;
	unsigned char currentLevel;
	long currentTime;
	unsigned char GetRunning();
	void AdjustViewpoint(int prevX,int prevY);

	long GetScore();
	void IncreaseScore(long value);

private:
	Viewpoint window;

	CHero*				hero;
	Level*				level;
	CharacterStatusBar*	statusBar; 
	HDC*				pBuffDC;

	long score;

	void PickUpGadget();
	void HandleKeys();
	void UpdateTimer();
	void EndGame();

	short keyHandler;
	short gadgetSelector;
};