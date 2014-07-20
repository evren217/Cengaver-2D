#pragma once

#include <Windows.h>

extern int debugmode; // tells the compiler this variable is declared elsewhere

extern char* TILESPROPERTIES;

extern int FRICTION_MAX;
extern int GRAVITY;

extern signed char HERO_MAX_HEALTH;

extern int PARALAXSPEED1;
extern int PARALAXSPEED2;
extern int ANIMATIONSPEED;

extern unsigned long CURRENTTIME;

extern HWND hWnd;

const unsigned char TILESIZE = 16; // The size of only one tile

// Collision
const unsigned char COLL_NONE	= 0x00;
const unsigned char COLL_TOP	= 0x01;
const unsigned char COLL_BOTTOM	= 0x02;
const unsigned char COLL_LEFT	= 0x04;
const unsigned char COLL_RIGHT	= 0x08;

const short WINDOWWIDTH		= 576; // The width of main window
const short WINDOWHEIGHT	= 320; // The height of main window

const COLORREF TRANSPARENCYCOLOR = RGB(255,0,255); // Transparency color of background tiles

const unsigned char ANIMATIONRATE = 4; // The general animation speed

const unsigned char INVENTORY_SIZE = 8; // Max size of items in inventory

// Level Max Size:
const unsigned char LEVEL_MAX_HEIGHT		= 64;
const short LEVEL_MAX_WIDTH		= 256;

// Hero State:
const unsigned char HS_NONE			= 20;
const unsigned char HS_WAITING		= 20;
const unsigned char HS_MOVELEFT		= 21;
const unsigned char HS_MOVERIGHT	= 22;
const unsigned char HS_DEAD			= 23;
const unsigned char HS_USEGADGET	= 20;

// Game Scroll:
const unsigned char SCROLL_RESOLUTION	= 25;
const short SCROLL_BORDERY				= 100;
const short SCROLL_BORDERX				= 200;

// Game Ending:
const unsigned char GAME_END_DEAD	= 1;
const unsigned char GAME_END_END	= 2;
const unsigned char GAME_END_LEVEL	= 3;

// Menu Constants:
const unsigned char MENU_SIZE	 = 4;
const unsigned char MENU_NEW	 = 0;
const unsigned char MENU_LOAD	 = 1;
const unsigned char MENU_CREDITS = 2;
const unsigned char MENU_EXIT	 = 3;
const unsigned char MENU_RESUME	 = 4;

// SplashScreen Max Duration:
const short MAX_TIME = 100;