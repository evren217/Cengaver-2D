#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define WINDOWWIDTH				576				// The width of main window
#define WINDOWHEIGHT			320				// The height of main window
#define TRANSPARENCYCOLOR		RGB(255,0,255)	// Transparency color of background tiles
#define TILESIZE				16				// The size of only one background tile

#define ANIMATIONRATE			4				// The general animation speed

// Hero States:
#define HS_NONE					20
#define HS_WAITING				20
#define HS_MOVELEFT				21
#define HS_MOVERIGHT			22
#define HS_DEAD					23
#define HS_USEGADGET			20

// Game Scroll:
#define SCROLL_RESOLUTION		25
#define SCROLL_BORDERY			100
#define SCROLL_BORDERX			200

// Collision
#define COLL_NONE				0x00
#define COLL_TOP				0x01
#define COLL_BOTTOM				0x02
#define COLL_LEFT				0x04
#define COLL_RIGHT				0x08

//LevelSize DO NOT CHANGE PLEASE
#define LEVEL_MAX_HEIGHT		64
#define LEVEL_MAX_WIDTH			256
#define MAX_LEVEL				2

//GameEnds
#define GAME_END_DEAD			1
#define GAME_END_END			2
#define GAME_END_LEVEL			3

#define INVENTORY_SIZE			8

//Menu constants
#define MENU_SIZE				4
#define MENU_NEW				0
#define MENU_LOAD				1
#define MENU_CREDITS			2
#define MENU_EXIT				3
#define MENU_RESUME				4

//Splash screen max duration
#define MAX_TIME				300

#endif