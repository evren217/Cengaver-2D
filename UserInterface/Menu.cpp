#include "Menu.h"
#include <Windows.h>
#include "../Utilities/Globals.h"

Menu::Menu(HDC* pBuffDC)
{
	keyHandler = -127; // Pressed RETURN Key (BAAADDD, but we have no chance!)

	isResumable = false;

	running = false;
	bufDC = pBuffDC;
	menuDeviceContext = CreateCompatibleDC(NULL);
	menuDeviceContext2 = CreateCompatibleDC(NULL);

	menuSelected = 0;
	menuActivated = 0;
	updateTimer = 0;
	newSelected = false;
	noGameRunning = true;

	rightBitmapPaths[0] = "Resources/Menu/menu-new.bmp";
	rightBitmapPaths[1] = "Resources/Menu/menu-load.bmp";
	rightBitmapPaths[2] = "Resources/Menu/menu-credits.bmp";
	rightBitmapPaths[3] = "Resources/Menu/menu-exit.bmp";

	leftBitmapPaths[0]	= "Resources/Menu/menu-left.bmp";
	leftBitmapPaths[1]	= "Resources/Menu/menu-left-resume.bmp";

	menuBitmapLeft = LoadImage(NULL, leftBitmapPaths[0], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	menuBitmapRight = LoadImage(NULL, rightBitmapPaths[0], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	SelectObject(menuDeviceContext,menuBitmapLeft); 
	SelectObject(menuDeviceContext2,menuBitmapRight);

	displayFont = CreateFont(-18, 0, 0, 0, FW_DONTCARE, false, false, false, OEM_CHARSET,OUT_DEFAULT_PRECIS, 
							  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE, 0);


	highscore = 0;
	lowtime = 99999*40;

	SetBkMode(*pBuffDC, TRANSPARENT);
	SetTextColor(*pBuffDC,RGB(255,255,255));
}

Menu::~Menu(void)
{
	DeleteDC(menuDeviceContext);
	DeleteDC(menuDeviceContext2);
	DeleteObject(menuBitmapLeft);
	DeleteObject(menuBitmapRight);
	DeleteObject(displayFont);
}

void Menu::HandleKeys()
{
	if(GetKeyState(VK_UP) & 0x8000)
	{
		if(GetKeyState(VK_UP) != keyHandler) {
			keyHandler = GetKeyState(VK_UP);
		
			SelectedUp();
		}
	}
	if(GetKeyState(VK_DOWN) & 0x8000)
	{
		if(GetKeyState(VK_DOWN) != keyHandler) {
			keyHandler = GetKeyState(VK_DOWN);
		
			SelectedDown();
		}
	}
	if(GetKeyState(VK_RETURN) & 0x8000)
	{
		if(GetKeyState(VK_RETURN) != keyHandler && GetKeyState(VK_RETURN) != keyHandler-1) {
			keyHandler = GetKeyState(VK_RETURN);
		
			running = false;
			newSelected = true;
			menuActivated = menuSelected;
		}
	}
	if(GetKeyState('r') & 0x8000 || GetKeyState('R') & 0x8000)
	{
		running = false;
		newSelected = true;
		menuActivated = MENU_RESUME;
	}
} 

void Menu::SetLeftMenu(bool resume)
{
	if (resume)
	{
		isResumable = true;
		DeleteObject(menuBitmapLeft);
		menuBitmapLeft = LoadImage(NULL,leftBitmapPaths[1] , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		SelectObject(menuDeviceContext,menuBitmapLeft);
	}
	else
	{
		isResumable = false;
		DeleteObject(menuBitmapLeft);
		menuBitmapLeft = LoadImage(NULL,leftBitmapPaths[0] , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		SelectObject(menuDeviceContext,menuBitmapLeft);
	}
}

void Menu::SelectedUp()
{
	// Decrease index:
	if(menuSelected == 0) menuSelected = (MENU_SIZE-1);
	else menuSelected--;

	// Switch bitmap:
	DeleteObject(menuBitmapRight);
	menuBitmapRight = LoadImage(NULL, rightBitmapPaths[menuSelected], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(menuDeviceContext2,menuBitmapRight);
}
void Menu::SelectedDown()
{
	// Increase index:
	menuSelected++;
	if(menuSelected == MENU_SIZE) menuSelected = 0;

	// Switch bitmap:
	DeleteObject(menuBitmapRight);
	menuBitmapRight = LoadImage(NULL, rightBitmapPaths[menuSelected], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(menuDeviceContext2,menuBitmapRight);
}

void Menu::Update()
{
	HandleKeys();
	updateTimer++;

	// Update key handler:
	if (updateTimer%40 == 0)
		keyHandler = 0;
	if(keyHandler == GetKeyState(VK_UP))
		if(!(GetKeyState(VK_UP) & 0x8000))
			keyHandler = 0;
	if(keyHandler == GetKeyState(VK_DOWN))
		if(!(GetKeyState(VK_DOWN) & 0x8000))
			keyHandler = 0;
	if(keyHandler == GetKeyState(VK_RETURN))
		if(!(GetKeyState(VK_RETURN) & 0x8000))
			keyHandler = 0;	
}

void Menu::Draw()
{
		int width = 288;
		int height = 320;

		TransparentBlt(*bufDC,0,0 ,width , height, menuDeviceContext, 0, 0, width, height, TRANSPARENCYCOLOR);

		TransparentBlt(*bufDC,width,0 ,width , height, menuDeviceContext2, 0, 0, width, height, TRANSPARENCYCOLOR);

		HGDIOBJ h = SelectObject(*bufDC,displayFont);
		
		char j[100], l[100], k[300] = "Highscore: ", w[300] = "";
		ltoa(highscore,j,10);
		ltoa(lowtime/40,l,10);
		
		strcat(k,j);
		strcat(k," Stars");
		strcat(w,l);
		strcat(w," Seconds");
		TextOut(*bufDC,  10, 10, k, strlen(k));
		
		SelectObject(*bufDC,h);
}

unsigned char Menu::GetMenuSelected()
{
	if(newSelected)
	{
		newSelected = false;
		return menuActivated;
	}
	return 99;
}

void Menu::SetHighscore(unsigned long newScore)
{
	if(newScore > highscore)
		highscore = newScore;
}

const unsigned long Menu::GetHighscore()
{
	return highscore;
}

void Menu::SetLowtime(unsigned long newTime)
{
	if(newTime < lowtime)
		lowtime = newTime;
}

const unsigned long Menu::GetLowtime()
{
	return lowtime;
}