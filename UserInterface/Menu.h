#pragma once
#include <Windows.h>

class Menu
{
public:
	Menu(HDC* pBuffDC);
	~Menu(void);

	bool running;
	bool noGameRunning;
	bool isResumable;

	void Draw();
	void Update();
	unsigned char GetMenuSelected();
	void SetLeftMenu(bool resume);
	void SetHighscore(unsigned long newScore);
	const unsigned long GetHighscore();
	void SetLowtime(unsigned long newTime);
	const unsigned long GetLowtime();

private:
	HDC *bufDC;	
	HDC menuDeviceContext;
	HDC menuDeviceContext2;

	HANDLE menuBitmapLeft;
	HANDLE menuBitmapRight;

	HFONT displayFont;

	unsigned char menuActivated;
	unsigned char menuSelected;
	unsigned short updateTimer;
	unsigned long highscore;
	unsigned long lowtime;
	bool newSelected;
	short keyHandler;
	
	const char* rightBitmapPaths[4];
	const char* leftBitmapPaths[2];

	void HandleKeys();
	void SelectedUp();
	void SelectedDown();
};