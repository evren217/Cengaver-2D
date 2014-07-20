#pragma once

#include <Windows.h>
#include <Vector>
#include "WorldRepository.h"
#include "../Utilities/Vector2D.hpp"
#include "../Utilities/CollisionPoint.hpp"
#include "../UserInterface/Viewpoint.h"
#include "../Utilities/Globals.h"

using namespace std;

class Enemy;

class Level
{
private:
	HANDLE	background1;
	HANDLE	paralaxBackground1;
	HANDLE	paralaxBackground2;
	HANDLE	Tilemap;
	HDC		levelDeviceContext;
	HDC		backGroundDeviceContext;
	HDC		paralaxBackground1DeviceContext;
	HDC		paralaxBackground2DeviceContext;
	HDC*	pBuffDC;

	int		paralax1Xpos;
	int		paralax1PrevHXPos;
	int		paralax1Distance;

	int		paralax2Xpos;
	int		paralax2PrevHXPos;
	int		paralax2Distance;

	unsigned char generalAnimationCounter;
	unsigned short generalAnimationIndent;

	vector<CollisionPoint> debugTilePoints;
	vector<CollisionPoint> debugFallPoints;

	void RaiseGeneralAnimationCounter();
	void DrawDebugCollisionTiles(Viewpoint window);
	bool CheckEnemyCollision();

	std::vector<CollisionPoint> cps;

public:
	Level(HDC* pBuffDC_p);
	virtual ~Level(void);

	unsigned char m_levelMap[LEVEL_MAX_HEIGHT][LEVEL_MAX_WIDTH]; 
	WorldRepository worldRep;

	CollisionPointGroup CreateBoxCollisionGroup(const CMovableObject* object, int distanceInPixels);
	CollisionPointGroup CheckCollision(CMovableObject* object);
	CollisionPointGroup CheckCollisionWithHero(CMovableObject* object);
	bool BoundingBoxCollision(CMovableObject* o, int left2, int right2, int top2, int bottom2) const;
	bool BoundingBoxCollision(CMovableObject* o, CMovableObject* p) const;
	bool CheckCollisionWithEnemies(CMovableObject* object);
	bool WillFall(CMovableObject* object, int steps) const;
	
	void Draw(Viewpoint windows,char layer);
	void DrawParalaxBackGrounds(Viewpoint window); 
	void ChangeParalaxX(CVector2D v,unsigned char layer);
	void AddCollisionPointToVector(CollisionPoint c);
	
	POINT Level::CheckDoor(CollisionPointGroup g);
	POINT CalculateTileModifiers(CollisionPointGroup g);
	bool CalculateTileAbsMod(CollisionPointGroup g);
	int CalcEnvironementDamage(CollisionPointGroup g);

	POINT FindBeginPoint();
	bool TouchEnd(CollisionPointGroup g);
	
	//void GenerateTestLevel(); // test level generation

	void readLevel(char* filename);

	void RemoveDoor(int x, int y, unsigned char toId1, unsigned char toId2);
	void RemoveFromRepository(Enemy* e);
};