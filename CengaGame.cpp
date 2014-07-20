#pragma once

#include <Windows.h>
#include "GameObjects/Game.h"
#include "UserInterface/Menu.h"
#include "Utilities/Macro.h"
#include "Utilities/Globals.h"
#include "Data/GameLoader.h"
#include "Data/ParamLoader.h"
#include "UserInterface/SplashScreen.h"
#include <time.h>
#include <stdlib.h>

// Attributes:
CGame *g;
Menu *m;
SplashScreen *s;

bool running;
long splashTime = 40*2;
BYTE keysPressed[256];

RECT scrnSize;
HDC bufDC, hDC;
HBITMAP bufBMP;
HANDLE hImage;
HWND hWnd,hWndSplash;

// Methods:
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void HandleMenuSelection();

// PROGRAM ENTRY POINT:
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// Main window programming using Win32 API:
	MSG msg;
	WNDCLASSEX wcex; // Win32 GUI
	HACCEL m_hAccelTable;

	// Load Params from ini File
	ParamLoader::Instance()->LoadParamsFromFile("Resources/Params.ini");

	// Main Window Properties:
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "Window";
	wcex.hIconSm		= 0;

	if (!RegisterClassEx(&wcex))
	{
		return FALSE;
	}

	// Create the main window:
	hWnd = CreateWindow("Window", "Cengaver The Prism Operation", WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX,
		GetSystemMetrics(SM_CXSCREEN)/2-WINDOWWIDTH/2, GetSystemMetrics(SM_CYSCREEN)/2-WINDOWHEIGHT/2, WINDOWWIDTH, WINDOWHEIGHT, NULL, NULL, hInstance, NULL);

	// Create the splash window:
	hWndSplash = CreateWindow("Window", "Cengaver Splash", WS_POPUPWINDOW | WS_EX_TOPMOST,
		GetSystemMetrics(SM_CXSCREEN)/2-576/2,GetSystemMetrics(SM_CYSCREEN)/2-320/2, 576, 320, hWnd, NULL, hInstance, NULL);

    if (!hWnd)
    {
		return FALSE;
    }

	if (!hWndSplash)
    {
 	  return FALSE;
    }

	hDC = ::GetDC(hWnd);
	::GetClientRect(hWnd, &scrnSize);
	bufDC = CreateCompatibleDC(NULL);
	bufBMP = CreateCompatibleBitmap(hDC, scrnSize.right, scrnSize.bottom);
	SelectObject(bufDC, bufBMP);
	DeleteObject(bufBMP);

	// Create menu and splash screen:
	m = new Menu(&bufDC);
	s = new SplashScreen(&bufDC);

	// Set highscore and lowtime loaded:
	m->SetHighscore(GameLoader().LoadGame()[3]);
	m->SetLowtime(GameLoader().LoadGame()[4]);
	
	// Input Handling:
	GetKeyboardState(keysPressed);

	ShowWindow(hWndSplash, nCmdShow);
	UpdateWindow(hWndSplash);
	
	// Main game loop:
	unsigned long time_count = 40; // Target FPS
	unsigned long long cur_time;
	unsigned long long perf_counter;
	unsigned long long next_time = 0;

	QueryPerformanceFrequency((LARGE_INTEGER*) &perf_counter);
	time_count = perf_counter/40;
	QueryPerformanceCounter((LARGE_INTEGER*) & next_time);

	// Blit picture on the splash screen and delete the resources used:
	HANDLE hh = LoadImage(NULL, "Resources/SplashScreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HDC hcd = CreateCompatibleDC(NULL);
	HDC hc = GetDC(hWndSplash);
	SelectObject(hcd,hh);
	BitBlt(hc, 0, 0, 576, 320, hcd, 0, 0, SRCCOPY);
	DeleteDC(hc);
	DeleteDC(hcd);
	DeleteObject(hh);

	srand((unsigned) time(0));

	// Gameloop and message handling system:
	running = true;
	while(running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Translate and dispatch the received message:
			if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} 
		else 
		{
			// Increase the query performance timer:
			QueryPerformanceCounter((LARGE_INTEGER*) &cur_time);

			// If timer goes off and in time frame:
			if(cur_time >= next_time)
			{
				// Set new time for query performance timer:
				next_time += time_count;
				if(next_time < cur_time)
					next_time = cur_time + time_count;

				// Splash screen:
				if(splashTime > 0)
				{
					splashTime--;
				}
				else if(splashTime == 0)
				{	
					// Delete splash screen:
					DestroyWindow(hWndSplash);
					splashTime = -1;
					hWndSplash = 0;
					// Show the main window:
					ShowWindow(hWnd, nCmdShow);
					UpdateWindow(hWnd);
				}
				else
				{
					// Update splash screen:
					if(s != 0 && s->running == true)
					{
						SplashScreens result = s->Update();
						if(result == SplashScreens::Credits)
						{
							m->running = true;
						}
						else if(result == SplashScreens::Dead)
						{
							m->running = true;
						}
						else if(result == SplashScreens::EndLevel)
						{
							g->running = true;
						}
						else if(result == SplashScreens::Splash)
						{
							m->running = true;
						}

						s->Draw();
					}
					
					// Update the game:
					if(g != 0 && g->running == true)
					{
						char result = g->Update();
						if(result == GAME_END_END)
						{
 							g->running = false;
							s->SetNewScreen(SplashScreens::EndGame);
							s->running = true;
							m->SetHighscore(g->GetScore());
							m->SetLowtime(g->currentTime);
							m->SetLeftMenu(false);
							GameLoader().SaveGame(0,0,HERO_MAX_HEALTH,m->GetHighscore(),m->GetLowtime());
						}
						else if(result == GAME_END_DEAD)
						{
							g->running = false;
							s->SetNewScreen(SplashScreens::Dead);
							s->running = true;
							m->SetLeftMenu(false);
						}
						else if(result == GAME_END_LEVEL)
						{
							g->running = false;
							GameLoader().SaveGame(g->currentLevel,g->currentTime,g->GetHero()->GetHealth(),m->GetHighscore(),m->GetLowtime());
							s->SetNewScreen(SplashScreens::EndLevel);
							s->running = true;
						}
						else
						{
							g->Draw();
						}
					}
					else
					{
						if(s->running == false)
							m->running = true;
					}
					
					// Update the menu:
					if(m != 0 && m->running == true)
					{
						m->Update();
						m->Draw();
						HandleMenuSelection();	
						if(running == false)
							break;
					}
					
					// Present the buffer:
					BitBlt(hDC, 0, 0, 576, 320, bufDC, 0, 0, SRCCOPY);
				}
			}
		}
	}
	
	// Delete the resources used:
	CLEAN(m)
	CLEAN(g)
	CLEAN(s)
	DeleteDC(bufDC);
	DeleteDC(hDC);

	::PostQuitMessage(0);
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_DESTROY:
			if(hWndSplash)
			{
				DestroyWindow(hWndSplash);
			}
			else
			{
				running = false;
			}
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void HandleMenuSelection()
{
	unsigned long* loadResult;
	switch(m->GetMenuSelected())
	{
		// Start new game:
		case MENU_NEW:
			CLEAN(g)
			m->SetLeftMenu(true);
			m->running = false;
			g = new CGame(&bufDC);
			GameLoader().SaveGame(1,0,HERO_MAX_HEALTH, -1, -1);
		break;
		
		// Load game:
		case MENU_LOAD:
			if (g != 0) {
				if (g->currentLevel != 0 && g->currentLevel != 1) {
					CLEAN(g)
					m->SetLeftMenu(true);
					m->running = false;
					g = new CGame(&bufDC);
					loadResult = GameLoader().LoadGame();
					g->currentLevel = (unsigned char)loadResult[0];
					g->currentTime = (long)loadResult[2];
					g->GetHero()->SetHealth((char)loadResult[1]);
					g->NextLevel();
				}
			}
		break;

		// Show credits:
		case MENU_CREDITS:
			s->SetNewScreen(Credits);
			s->running = true;
			m->running = false;
		break;

		// Quit:
		case MENU_EXIT:
			running = false;
		break;

		// Resume:
		case MENU_RESUME:
			if(g != 0 && m->isResumable)
				g->running = true;
		break;

		default:
		break;
	}
}