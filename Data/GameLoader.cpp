#include "GameLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shlobj.h>
#include <direct.h>

GameLoader::GameLoader(void)
{
}

GameLoader::~GameLoader(void)
{
}

unsigned long* GameLoader::LoadGame()
{
	// Open file:
	FILE* file  = 0;
	unsigned long returnVal[5] = {0,10,0,0,99999*40};

	char path[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, NULL, path);
	strcat(path, "\\");
	strcat(path, "AyselEvren");
	_mkdir(path);
	strcat(path, "\\");
	strcat(path, "Cengaver The Prism Operation");
	_mkdir(path);
	strcat(path, "\\");
	strcat(path, "Save.sav");
	
	fopen_s(&file, path, "r");
	if(file)
	{
		// Read level:
		returnVal[0] = fgetc(file);
		// Read health:
		returnVal[1] = fgetc(file);
		// Read time:
		char j[50];
		fread(j,1,sizeof(j),file);
		returnVal[2] = atol(j);
		// Read highscore:
		char h[50];
		fread(h,1,sizeof(h),file);
		returnVal[3] = atol(h);
		// Read lowtime:
		char l[50];
		fread(l,1,sizeof(l),file);
		returnVal[4] = atol(l);
		if(returnVal[4] == 0)
			returnVal[4] = 99999*40;
		// Close file:
		fclose(file);
	}
	return returnVal; 
}

void GameLoader::SaveGame(char i, unsigned long time,unsigned char health, long highscore, long lowtime)
{
	// Open file:
	FILE* file = 0;

	char path[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, NULL, path);
	strcat(path, "\\");
	strcat(path, "AyselEvren");
	_mkdir(path);
	strcat(path, "\\");
	strcat(path, "Cengaver The Prism Operation");
	_mkdir(path);
	strcat(path, "\\");
	strcat(path, "Save.sav");

	fopen_s(&file, path, "w+");
	if(file)
	{
		// Write level id:
		fputc(i,file);
		// Write health:
		fputc(health,file);
		// Write time:
		char t[50];
		itoa(time,t,10);
		fwrite(t,strlen(t),sizeof(t),file);
		// Write highscore:
		if(highscore > 0)
		{
			char h[50];
			itoa(highscore,h,10);
			fwrite(h,strlen(h),sizeof(h),file);
		}
		// Write lowtime:
		if(lowtime > 0)
		{
			char l[50];
			itoa(lowtime,l,10);
			fwrite(l,strlen(l),sizeof(l),file);
		}
		// Close file:
		fclose(file);
	}
}