#pragma once

class GameLoader
{
public:
	GameLoader(void);
	~GameLoader(void);

	unsigned long* LoadGame();
	void SaveGame(char i, unsigned long time, unsigned char health, long highscore, long lowtime);
};