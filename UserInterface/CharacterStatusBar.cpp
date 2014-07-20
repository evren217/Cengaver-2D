#include "CharacterStatusBar.h"
#include "../Utilities/Globals.h"
#include <Windows.h>

CharacterStatusBar::CharacterStatusBar(HDC* pBuffDC_p)
{
	pBuffDC = pBuffDC_p;	

	SetTextColor(*pBuffDC,RGB(255,255,255));
}

CharacterStatusBar::~CharacterStatusBar(void)
{
}

void CharacterStatusBar::Draw(unsigned char health, unsigned long time, unsigned char score)
{
	const char * j;

	// Draw the score:
	j = IntToString(score);
	TextOut(*pBuffDC,526,0,"Score",strlen("Score"));
	TextOut(*pBuffDC,536,15,j,strlen(j));

	// Draw the health:
	j = IntToString(health);
	TextOut(*pBuffDC,68,0,"Health",strlen("Health"));
	TextOut(*pBuffDC,78,15,j,strlen(j));

	// Draw the time played:
	j = IntToString(time/40);
	TextOut(*pBuffDC,9,0,"Time",strlen("Time"));
	TextOut(*pBuffDC,15,15,j,strlen(j));
}

const char* CharacterStatusBar::IntToString(unsigned long number)
{
	char j[100];
	itoa(number,j,10);
	return j;
}