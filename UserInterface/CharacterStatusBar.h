#pragma once

#include <Windows.h>

class CharacterStatusBar
{
public:
	CharacterStatusBar(HDC* pBuffDC_p);
	~CharacterStatusBar(void);

	void Draw(unsigned char health, unsigned long time, unsigned char score);

private:
	HDC*	pBuffDC;

	const char* IntToString(unsigned long time);
};