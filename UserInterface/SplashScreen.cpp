#include "SplashScreen.h"
#include "../Utilities/Globals.h"
#include <Windows.h>

#define UPDATE_TIME 3

SplashScreen::SplashScreen(HDC* pBuffDC_p)
{
	keyHandler = 0;
	
	// Save double buffer device context:
	pBuffDC = pBuffDC_p;
	
	// Create compatible device context:
	splashDC = CreateCompatibleDC(NULL);
	
	// Get bitmaps:
	splashBitmap = LoadImage(NULL, "Resources/Splashscreen2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	gameoverBitmap = LoadImage(NULL, "Resources/Gameover.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	creditBitmap = LoadImage(NULL, "Resources/Credits.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	endLevelBitmap = LoadImage(NULL, "Resources/EndLevel.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	endGameBitmap = LoadImage(NULL, "Resources/EndGame.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	// Reset running:
	running = true;
	
	// Set default splash:
	SetNewScreen(Splash);
}

SplashScreen::~SplashScreen(void)
{
	DeleteDC(splashDC);
	DeleteObject(splashBitmap);
	DeleteObject(gameoverBitmap);
	DeleteObject(creditBitmap);
	DeleteObject(endLevelBitmap);
	DeleteObject(endGameBitmap);
}

SplashScreens SplashScreen::Update()
{
	if(updateTimer >= UPDATE_TIME)
	{
		if (currentSplash == SplashScreens::Splash) {
			if(GetKeyState(VK_RETURN) & 0x8000) {
				if(GetKeyState(VK_RETURN) != keyHandler) {
					keyHandler = GetKeyState(VK_RETURN);
					timeLeft = 0;
				}
			}
		}
		updateTimer = 0;
	}
	else
	{
		updateTimer++;

		if (currentSplash == SplashScreens::Splash) {
			// Update key handler:
			if(keyHandler == GetKeyState(VK_RETURN))
			if(!(GetKeyState(VK_RETURN) & 0x8000))
				keyHandler = 0;
		}
	}
	timeLeft--;
	
	if(timeLeft <= 0 || timeLeft > MAX_TIME)
	{
		running = false;
		return currentSplash;
	}
	return SplashScreens::None;
}

void SplashScreen::SetNewScreen(SplashScreens s)
{
	switch(s)
	{
		case SplashScreens::Splash:
			SelectObject(splashDC,splashBitmap);
			currentSplash = SplashScreens::Splash;
		break;
		case SplashScreens::Credits:
			SelectObject(splashDC,creditBitmap);
			currentSplash = SplashScreens::Credits;
		break;
		case SplashScreens::Dead:
			SelectObject(splashDC,gameoverBitmap);
			currentSplash = SplashScreens::Dead;
		break;
		case SplashScreens::EndLevel:
			SelectObject(splashDC,endLevelBitmap);
			currentSplash = SplashScreens::EndLevel;
		break;
		case SplashScreens::EndGame:
			SelectObject(splashDC,endGameBitmap);
			currentSplash = SplashScreens::EndGame;
		break;
	}

	timeLeft = MAX_TIME;
}

void SplashScreen::Draw()
{
	TransparentBlt(*pBuffDC,0,0 ,576 , 320, splashDC, 0, 0, 576, 320, TRANSPARENCYCOLOR);
}