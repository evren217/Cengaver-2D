#pragma once
#include <Windows.h>

enum SplashScreens
{
	None,
	Splash,
	Credits,
	Dead,
	EndLevel,
	EndGame,
};

class SplashScreen
{
public:
	SplashScreen(HDC* pBuffDC_p);
	~SplashScreen(void);

	bool running;
	
	SplashScreens Update();
	void Draw();
	void SetNewScreen(SplashScreens s);

private:
	HANDLE	splashBitmap;
	HANDLE	creditBitmap;
	HANDLE	gameoverBitmap;
	HANDLE	endLevelBitmap;
	HANDLE	endGameBitmap;
	HDC		splashDC;
	HDC*	pBuffDC;
	
	unsigned short timeLeft;
	unsigned short updateTimer;
	short keyHandler;
	SplashScreens currentSplash; 
};