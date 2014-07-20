#include "Level.h"
#include <Algorithm>
#include <Vector>
#include <Windows.h>
#include <sstream>
#include <string>
#include "../Data/TileManager.h"
#include "../Data/TileProperties.h"
#include "../Utilities/CollisionPoint.hpp"
#include "../Utilities/Macro.h"
#include "../Utilities/Vector2D.hpp"
#include "../Utilities/Globals.h"
#include "../Data/IniReader.h"
#include "Gadgets/Health.hpp"
#include "Gadgets/Star.hpp"
#include "Gadgets/Key.hpp"
#include "Gadgets/Prism.hpp"

#include <stdio.h>

using namespace std;

Level::Level(HDC* pBuffDC_p)
{
	// Load Bitmaps:
	background1 = LoadImage(NULL, "Resources/MainBackground.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Tilemap = LoadImage(NULL, "Resources/BackgroundTiles.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	paralaxBackground1 = LoadImage(NULL, "Resources/paralaxBackground1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	paralaxBackground2 = LoadImage(NULL, "Resources/paralaxBackground2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	// Create Device Contexts:
	pBuffDC = pBuffDC_p;
	levelDeviceContext = CreateCompatibleDC(*pBuffDC);
	backGroundDeviceContext = CreateCompatibleDC(*pBuffDC);
	paralaxBackground1DeviceContext = CreateCompatibleDC(*pBuffDC);
	paralaxBackground2DeviceContext = CreateCompatibleDC(*pBuffDC);

	// Select Handles:
	SelectObject(levelDeviceContext, Tilemap);
	SelectObject(backGroundDeviceContext, background1);
	SelectObject(paralaxBackground1DeviceContext,paralaxBackground1);
	SelectObject(paralaxBackground2DeviceContext,paralaxBackground2);

	// Reset Parallax:
	paralax1Xpos = 0;
	paralax2Xpos = 0;

	paralax1Distance = PARALAXSPEED1;
	paralax2Distance = PARALAXSPEED2;

	// Reset Tile Animation:
	generalAnimationCounter = 0;
	generalAnimationIndent = 0;
}

Level::~Level(void)
{
	DeleteDC(levelDeviceContext);
	DeleteDC(backGroundDeviceContext);
	DeleteDC(paralaxBackground1DeviceContext);
	DeleteDC(paralaxBackground2DeviceContext);
	DeleteObject(background1);
	DeleteObject(Tilemap);
	DeleteObject(paralaxBackground1);
	DeleteObject(paralaxBackground2);
}

//void Level::GenerateTestLevel(void)
//{
//	// Initialize level map:
//	for(int i=0; i<LEVEL_MAX_HEIGHT; i++)
//	{
//		for(int j=0; j<LEVEL_MAX_WIDTH; j++)
//		{
//			m_levelMap[i][j] = 0;
//		}
//	}
//
//	// Create a simple level map:
//	for(int i=0; i<LEVEL_MAX_WIDTH; i++)
//	{
//		// Begin and end of the level:
//		// Begin tile must be not solid, End tile must be solid
//		// 88 is begin, 89 is end
//		m_levelMap[80][10] = 88;
//		m_levelMap[88][60] = 89;
//
//		// Ground Floor:
//		m_levelMap[95][i] = 1;
//
//		// Simple Walls:
//		if(i < 5)
//		{
//			m_levelMap[85][i] = 1;
//			m_levelMap[86][i] = 2;
//			m_levelMap[87][i] = 2;
//			m_levelMap[88][i] = 2;
//			m_levelMap[89][i] = 2;
//			m_levelMap[90][i] = 2;
//			m_levelMap[91][i] = 2;
//			m_levelMap[92][i] = 2;
//			m_levelMap[93][i] = 2;
//			m_levelMap[94][i] = 2;
//			m_levelMap[95][i] = 2;
//		}
//		if(i > 50)
//		{
//			m_levelMap[92][i] = 1;
//			m_levelMap[93][i] = 2;
//			m_levelMap[94][i] = 2;
//			m_levelMap[95][i] = 2;
//		}
//		if(i > 70)
//		{
//			m_levelMap[85][i] = 1;
//			m_levelMap[86][i] = 2;
//			m_levelMap[87][i] = 2;
//			m_levelMap[88][i] = 2;
//			m_levelMap[89][i] = 2;
//			m_levelMap[90][i] = 2;
//			m_levelMap[91][i] = 2;
//			m_levelMap[92][i] = 2;
//		}
//		
//
//		// Ground Platform:
//		for(int j=96; j<100; j++)
//		{
//			m_levelMap[j][i] = 2;
//		}
//	}
//
//	// TEST Sync Tile Animation:
//	for(int i=0; i<LEVEL_MAX_WIDTH; i++)
//	{
//		if(i%9==0)
//			m_levelMap[96][i] = 4;
//	}
//}

void Level::DrawParalaxBackGrounds(Viewpoint window)
{
	// Draw background 1:
	TransparentBlt(*pBuffDC ,0 ,0,
					600, 350, backGroundDeviceContext, 0, 0, 600, 
					350, TRANSPARENCYCOLOR);

	// Draw paralax background 2 (back):
	TransparentBlt(*pBuffDC  ,0-paralax2Xpos ,0,600, 350, paralaxBackground2DeviceContext, 0, 0, 600, 350, RGB(80,179,247));
	TransparentBlt(*pBuffDC  ,window.width-paralax2Xpos ,0,600, 350, paralaxBackground2DeviceContext, 0, 0, 600, 350, RGB(80,179,247));
	TransparentBlt(*pBuffDC  ,0-window.width-paralax2Xpos ,0,600, 350, paralaxBackground2DeviceContext, 0, 0, 600, 350, RGB(80,179,247));

	// Draw paralax background 1 (front):
	TransparentBlt(*pBuffDC  ,0-paralax1Xpos ,0,600, 350, paralaxBackground1DeviceContext, 0, 0, 600, 350, RGB(255,255,255));
	TransparentBlt(*pBuffDC  ,window.width-paralax1Xpos ,0,600, 350, paralaxBackground1DeviceContext, 0, 0, 600, 350, RGB(255,255,255));
	TransparentBlt(*pBuffDC  ,0-window.width-paralax1Xpos ,0,600, 350, paralaxBackground1DeviceContext, 0, 0, 600, 350, RGB(255,255,255));
} 

void Level::Draw(Viewpoint window,char layer)
{
	// Draw if within viewpoint range:
	for(int i=window.viewY/TILESIZE, is = 0;i*TILESIZE < window.viewY+window.height && i<LEVEL_MAX_HEIGHT; i++, is+=TILESIZE)
	{
		for(int j=window.viewX/TILESIZE,js = 0;j*TILESIZE < window.viewX+window.width && j<LEVEL_MAX_WIDTH; j++, js+=TILESIZE)
		{
			if(m_levelMap[i][j] != 0)
			{
				// Select tile property:
				TileProperties tp = TileManager::Instance()->GetTileProperties(m_levelMap[i][j]);

				if(tp.layer == layer)
				{
					// Set modifier:
					unsigned short xModifier = 0;
					if((tp.variation == true && tp.animation == false )|| tp.variation == true)
						xModifier += i*47+j*41;
					if(tp.animation == true)
						xModifier += generalAnimationCounter;

					// Transparen blit on double buffer:
					TransparentBlt(*pBuffDC,
									js-window.viewX%TILESIZE,
									is-window.viewY%TILESIZE,
									tp.bitmapWidth,
									tp.bitmapHeight,
									levelDeviceContext, 
									((tp.bitmapXPos+xModifier)%tp.bitmapCount)*tp.bitmapWidth, 
									tp.bitmapYPos*tp.bitmapHeight, 
									tp.bitmapWidth, tp.bitmapHeight,
									TRANSPARENCYCOLOR);
				}
			}
		}
	}
	if(layer == -1)
		RaiseGeneralAnimationCounter();

	if(debugmode == 1)
	{
		DrawDebugCollisionTiles(window);
		
		HPEN hPen = CreatePen(PS_DOT, 1, RGB(255,0,0));
		SelectObject(levelDeviceContext, GetStockObject(NULL_BRUSH));
		SelectObject(levelDeviceContext, hPen);
		
		std::vector<CollisionPoint>::iterator itr;
		for ( itr = cps.begin(); itr != cps.end(); ++itr )
		{
			Rectangle(*pBuffDC,((*itr).x-window.viewX)-2,((*itr).y-window.viewY)-2,((*itr).x-window.viewX)+2,((*itr).y-window.viewY)+2); 
		}
		DeleteObject(hPen);
	}
}

bool CollisionPointSorter (CollisionPoint i,CollisionPoint j)
{ 
	return (i.pseudoDistance<j.pseudoDistance && i.pseudoDistance>0); 
}

bool CollisionPointIsValid (CollisionPoint i) 
{ 
	return i.x>0 && i.y>0 && i.x < LEVEL_MAX_WIDTH*TILESIZE && i.y < LEVEL_MAX_HEIGHT*TILESIZE;
}

bool Level::BoundingBoxCollision(CMovableObject* o, int left2, int right2, int top2, int bottom2) const
{
    int left1;
    int right1;
    int top1;
    int bottom1;

	left1 = o->X();
	top1  = o->Y();

	right1  = left1 + o->Width();
	bottom1 = top1  + o->Height();

    if (bottom1 < top2) return false;
    if (top1 > bottom2) return false;
    if (right1 < left2) return false;
    if (left1 > right2) return false;

    return true;
}

bool Level::BoundingBoxCollision(CMovableObject* o, CMovableObject* p) const
{
    int left1, left2;
    int right1, right2;
    int top1, top2;
    int bottom1, bottom2;

	left1 = o->X();
	left2 = p->X();

	right1 = left1 + o->Width();
	right2 = left2 + p->Width();

	top1 = o->Y();
	top2 = p->Y();

	bottom1 = top1 + o->Height();
	bottom2 = top2 + p->Height();

    if (bottom1 < top2) return false;
    if (top1 > bottom2) return false;
    if (right1 < left2) return false;
    if (left1 > right2) return false;

    return true;
}

bool Level::CheckCollisionWithEnemies(CMovableObject* object)
{
	CollisionPointGroup collissions;
	

	CollisionPoint cp;
	cp.isEnemy = true;

	bool coll = false;

	for (int i = 0; i < worldRep.enemies.size(); i++)
	{
		if (worldRep.enemies[i] == NULL)
		{
			continue;
		}

		coll = BoundingBoxCollision(object, worldRep.enemies[i]);

		int direction = object->movementVector.DirectionAsConstants();

		if (coll && CONTAINS(direction, COLL_BOTTOM))
		{
			// Hurt the enemy:
			if(worldRep.enemies[i]->tileID == 33)
			{
				bool bossKilled = worldRep.enemies[i]->TakeBossHit();
				object->movementVector.SetDY(-15);
				if (bossKilled)
					Level::worldRep.AddGadget(new Key(pBuffDC, worldRep.enemies[i]->X(), worldRep.enemies[i]->Y(), false, 72));
				continue;
			}
			else
			{
				worldRep.enemies[i]->TakeHit(1);
				object->movementVector.SetDY(-10);
				continue;
			}
		}
		else if (coll != COLL_NONE || coll && CONTAINS(direction, COLL_RIGHT) || coll && CONTAINS(cp.collisionDirection, COLL_LEFT))
		{
			// Enemy hurts the hero:
			cp.x = worldRep.enemies[i]->X();
			cp.y = worldRep.enemies[i]->Y();
			
			// Bounce back:
			if(object->movementVector.DX()>0)
				object->movementVector.SetDX(-object->movementVector.DX());
			else if(object->movementVector.DX()<0)
				object->movementVector.SetDX(-object->movementVector.DX());
			else if(worldRep.enemies[i]->movementVector.DX()<0)
				object->movementVector.SetDX(worldRep.enemies[i]->movementVector.DX()-2);
			else if(worldRep.enemies[i]->movementVector.DX()>0)
				object->movementVector.SetDX(worldRep.enemies[i]->movementVector.DX()+2);
			
			collissions.Add(cp);

			if (worldRep.enemies[i]->tileID == 30)
				object->TakeHit(1);
			if (worldRep.enemies[i]->tileID == 31)
				object->TakeHit(2);
			if (worldRep.enemies[i]->tileID == 32)
				object->TakeHit(1);
			if (worldRep.enemies[i]->tileID == 33)
				object->TakeHit(1);
		}
	}
	if (collissions.Size() > 0)
	{
		return true;
	}

	return false;
}

CollisionPointGroup Level::CheckCollision(CMovableObject* object)
{
	CollisionPointGroup cpg = CreateBoxCollisionGroup(object, object->movementVector.Length()/TILESIZE);
	CollisionPointGroup collissions;
	cpg.CopyTo(&debugTilePoints);

	vector<CollisionPoint>::iterator it;
	for(it = cpg.Begin(); it < cpg.End(); it++)
	{
		CollisionPoint cp = *it;

		unsigned char ch = m_levelMap[cp.y][cp.x];
		bool solid = TileManager::Instance()->GetTileProperties( ch ).solid;
		bool validpoint = CollisionPointIsValid(cp);

		if((TileManager::Instance()->GetTileProperties(ch).healthMod != 0 || TileManager::Instance()->GetTileProperties(ch).xMod != 0 || TileManager::Instance()->GetTileProperties(ch).yMod != 0) && TileManager::Instance()->GetTileProperties(ch).id != 0 && !solid)
		{
			collissions.Add(cp);
		}
		else if(validpoint && solid)
		{
			
			if(CONTAINS(cp.collisionDirection, COLL_RIGHT))
			{
				if(object->movementVector.NewX() > (cp.x-(object->Width()/TILESIZE))*TILESIZE - TILESIZE)
				{
					object->movementVector.SetX((cp.x-(object->Width()/TILESIZE))*TILESIZE - TILESIZE);
					object->movementVector.SetDX(0);
					collissions.Add(cp);
				}
			}
			else if(CONTAINS(cp.collisionDirection, COLL_LEFT))
			{
				if(object->movementVector.NewX() < (cp.x+(object->Width()/TILESIZE))*TILESIZE)
				{
					object->movementVector.SetX((cp.x+(object->Width()/TILESIZE))*TILESIZE);
					object->movementVector.SetDX(0);
					collissions.Add(cp);
				}
			}

			if(CONTAINS(cp.collisionDirection, COLL_BOTTOM))
			{
				if(object->movementVector.NewY() > (cp.y-(object->Height()/TILESIZE))*TILESIZE)
				{
					object->movementVector.SetY((cp.y-(object->Height()/TILESIZE))*TILESIZE);
					object->movementVector.SetDY(0);
					collissions.Add(cp);
				}
			} 
			else if(CONTAINS(cp.collisionDirection, COLL_TOP))
			{
				if(object->movementVector.NewY() < (cp.y+(object->Height()/TILESIZE))*TILESIZE)
				{
					object->movementVector.SetY((cp.y+(object->Height()/TILESIZE))*TILESIZE);
					object->movementVector.SetDY(0);
					collissions.Add(cp);
				}
			}
		}
	}

	return collissions;
}

CollisionPointGroup Level::CreateBoxCollisionGroup(const CMovableObject* object, int distanceInTiles)
{
	int leftO, rightO, topO, bottomO;

	leftO = object->X();
	rightO = object->X() + object->Width();
	topO = object->Y();
	bottomO = object->Y() + object->Height();

	CollisionPointGroup cpg = CollisionPointGroup();
	
	if(TileManager::Instance()->GetTileProperties(m_levelMap[object->movementVector.Y()/TILESIZE][object->movementVector.X()/TILESIZE]).solid == false &&
		TileManager::Instance()->GetTileProperties(m_levelMap[object->movementVector.Y()/TILESIZE][object->movementVector.X()/TILESIZE]).id != 0)
			cpg.Add(object->movementVector.X()/TILESIZE,object->movementVector.Y()/TILESIZE,COLL_TOP);
	if(TileManager::Instance()->GetTileProperties(m_levelMap[object->movementVector.Y()/TILESIZE+1][object->movementVector.X()/TILESIZE]).solid == false && 
		TileManager::Instance()->GetTileProperties(m_levelMap[object->movementVector.Y()/TILESIZE+1][object->movementVector.X()/TILESIZE]).id != 0)
			cpg.Add(object->movementVector.X()/TILESIZE,(object->movementVector.Y()/TILESIZE)+1,COLL_TOP);
	
	if(object->movementVector.Length() == 0)
		return cpg;

	int direction = object->movementVector.DirectionAsConstants();
	int fX = 0, fY = 0;
	
	if(CONTAINS(direction, COLL_LEFT))
	{
		for(int y = topO/TILESIZE; y <= (bottomO-1)/TILESIZE; y++)
		{
			cpg.Add((leftO)/TILESIZE, y, COLL_LEFT);
		}
		fX--;
	}
	if(CONTAINS(direction, COLL_RIGHT))
	{
		for(int y = topO/TILESIZE; y < bottomO/TILESIZE; y++)
		{
			cpg.Add((rightO)/TILESIZE, y, COLL_RIGHT);
		}
		fX++;
	}
	if(CONTAINS(direction, COLL_TOP))
	{
		for(int x = leftO/TILESIZE; x <= (rightO-1)/TILESIZE; x++)
		{
			cpg.Add(x, topO/TILESIZE - 1, COLL_TOP);
		}
		fY--;
	}
	
	for(int x = leftO/TILESIZE; x <= (rightO-1)/TILESIZE; x++)
	{
		cpg.Add(x, bottomO/TILESIZE, COLL_BOTTOM);
	}
	fY++;
	
	cpg.AddByMoving(fX, fY, distanceInTiles);
	
	vector<CollisionPoint>::iterator it;
	for(it = cpg.Begin(); it < cpg.End(); it++)
	{
		(*it).pseudoDistance = (object->X() - ((*it).x*TILESIZE))* (object->X() - ((*it).x*TILESIZE)) +
			(object->Y() - ((*it).y*TILESIZE)) * (object->Y() - ((*it).y*TILESIZE));
	}

	if(cpg.Size() > 0)
	{
		std::sort(cpg.Begin(), cpg.End(), CollisionPointSorter);
		
		CollisionPoint cp = *cpg.Begin();
		for(it = cpg.Begin()+1; it < cpg.End(); it++)
		{
			if(cp == *it)
			{
				cp.collisionDirection |= (*it).collisionDirection;
				(*it).collisionDirection |= cp.collisionDirection;
			}
			cp = *it;
		}
	}

	return cpg;
}

bool Level::WillFall(CMovableObject* object, int steps) const
{
	int leftO = object->X();
	int rightO = object->X() + object->Width();
	int bottomO = object->Y() + object->Height();

	int direction = object->movementVector.DirectionAsConstants();
	if(CONTAINS(direction, COLL_LEFT))
		for(int x = 0; x < steps; x++)
			if(false == TileManager::Instance()->GetTileProperties( m_levelMap[(bottomO)/TILESIZE][(leftO/TILESIZE)-x]).solid)
				return true;
	
	if(CONTAINS(direction, COLL_RIGHT))
		for(int x = 0; x < steps; x++)
			if(false == TileManager::Instance()->GetTileProperties( m_levelMap[(bottomO)/TILESIZE][(rightO/TILESIZE)+x]).solid)
				return true;
	
	return false;
}

void Level::ChangeParalaxX(CVector2D v,unsigned char layer)
{
	int* paralaxScrollDistance;
	int* paralaxPrevHPos;
	int* paralaxXPos;

	if(layer == 1)
	{
		paralaxScrollDistance = &paralax1Distance;
		paralaxPrevHPos = &paralax1PrevHXPos;	 
		paralaxXPos = &paralax1Xpos;
	}
	if(layer == 2)
	{
		paralaxScrollDistance = &paralax2Distance;
		paralaxPrevHPos = &paralax2PrevHXPos;	 
		paralaxXPos = &paralax2Xpos;
	}

	if(v.X()-*paralaxPrevHPos > *paralaxScrollDistance)
	{
		if(*paralaxXPos<WINDOWWIDTH)
			*paralaxXPos+=1;
		else
			*paralaxXPos = 0;
		*paralaxPrevHPos = v.X();
	}
	if(v.X()-*paralaxPrevHPos < -*paralaxScrollDistance)
	{
		if(*paralaxXPos>-WINDOWWIDTH)
			*paralaxXPos-=1;
		else
			*paralaxXPos = 0;
		*paralaxPrevHPos = v.X();
	}
}

void Level::RaiseGeneralAnimationCounter()
{
	if(generalAnimationIndent==ANIMATIONRATE)
	{
		if(generalAnimationCounter<7)
			generalAnimationCounter++;
		else
			generalAnimationCounter = 0;
		generalAnimationIndent = 0;
	}
	else
	{
		generalAnimationIndent++;
	}
}

void Level::DrawDebugCollisionTiles(Viewpoint viewpoint)
{
	// Setup rectangle structure:
	RECT r;
	
	// Create solid brush:
	HBRUSH b = CreateSolidBrush(RGB(255,0,0));

	// Draw the points on the screen:
	for(int i=0;i<debugTilePoints.size();i++)
	{
		r.left = debugTilePoints[i].x*TILESIZE - viewpoint.viewX;
		r.top = debugTilePoints[i].y*TILESIZE  - viewpoint.viewY;
		r.right = debugTilePoints[i].x*TILESIZE - viewpoint.viewX + TILESIZE;
		r.bottom = debugTilePoints[i].y*TILESIZE  - viewpoint.viewY + TILESIZE;
		FillRect(*pBuffDC,&r,b);
	}

	// Delete solid brush:
	DeleteObject(b);

	// Clear the vector:
	debugTilePoints.clear();
}

void Level::AddCollisionPointToVector(CollisionPoint c)
{
	debugTilePoints.push_back(c);
}

POINT Level::CalculateTileModifiers(CollisionPointGroup g)
{
	POINT returnValue;
	returnValue.x = 0;
	returnValue.y = 0;
	
	vector<CollisionPoint>::iterator it;
	for(it = g.Begin(); it < g.End(); it++)
	{
		if(m_levelMap[(*it).y][(*it).x] != 0)
		{
			TileProperties tp = TileManager::Instance()->GetTileProperties(m_levelMap[(*it).y][(*it).x]);
			returnValue.x += tp.xMod;
			returnValue.y += tp.yMod;
		}
	}
	return returnValue;
}

bool Level::CalculateTileAbsMod(CollisionPointGroup g)
{
	vector<CollisionPoint>::iterator it;
	for(it = g.Begin(); it < g.End(); it++)
	{
		TileProperties tp = TileManager::Instance()->GetTileProperties(m_levelMap[(*it).y][(*it).x]);
		if(tp.absMod)
			return true;
	}
	return false;
}

POINT Level::FindBeginPoint()
{
	POINT returnPoint;
	returnPoint.x = 5;
	returnPoint.y = 5;

	for(int i=0;i<LEVEL_MAX_HEIGHT;i++)
		for(int j=0;j<LEVEL_MAX_WIDTH;j++)
			if(m_levelMap[i][j] == 88)
			{
				returnPoint.x = j; 
				returnPoint.y = i;
				return returnPoint;
			}
	return returnPoint;
}

bool Level::TouchEnd(CollisionPointGroup g)
{
	vector<CollisionPoint>::iterator it;
	for(it = g.Begin(); it < g.End(); it++)
	{
		if(m_levelMap[(*it).y][(*it).x]!= 0)
		{
			TileProperties tp = TileManager::Instance()->GetTileProperties(m_levelMap[(*it).y][(*it).x]);
			if(tp.id == 89)
				return true;
		}
	}
	return false;
}

int Level::CalcEnvironementDamage(CollisionPointGroup g)
{
	int totalHealthMod = 0;
	vector<CollisionPoint>::iterator it;
	for(it = g.Begin(); it < g.End(); it++)
	{
		if(m_levelMap[(*it).y][(*it).x]!= 0)
		{
			TileProperties tp = TileManager::Instance()->GetTileProperties(m_levelMap[(*it).y][(*it).x]);
			totalHealthMod += tp.healthMod;
		}
	}
	return totalHealthMod;
}  

void Level::readLevel(char* filename)
{
	CIniReader iniReader(filename);
	unsigned char count = iniReader.SectionCount();
	char* sections = iniReader.GetSections();

	char* enemyPath  = "Resources/Enemies.bmp";
	char* gadgetPath = "Resources/Gadgets.bmp";

	for(int i=0; i<count; i++)
	{
		if(strstr(sections, "enemy") == sections)
		{
			// Enemies:
			Enemy* enemy = new Enemy(pBuffDC, 5, 5, enemyPath, iniReader.ReadInteger(sections,"id", 0),
							CVector2D(
								iniReader.ReadInteger(sections, "x", 0) * TILESIZE,
								iniReader.ReadInteger(sections, "y", 0) * TILESIZE
								));
			enemy->SetAILevel(this);
			worldRep.AddEnemy(enemy);
		}
		else if(strstr(sections, "gadget") == sections)
		{
			// Gadgets:
			int id = iniReader.ReadInteger(sections, "id", 0);
			
			int x = iniReader.ReadInteger(sections, "x", 0) * TILESIZE;
			int y = iniReader.ReadInteger(sections, "y", 0) * TILESIZE;

			switch(id){
				case 70: // Star gadget
					worldRep.AddGadget(new Star(pBuffDC, x, y, false, id));
					break;
				case 71: // Health gadget
					worldRep.AddGadget(new Health(pBuffDC, x, y, false, id));
					break;
				case 72: // Key gadget
					worldRep.AddGadget(new Key(pBuffDC, x, y, false, id));
					break;
				case 73: // Prism gadget
					worldRep.AddGadget(new Prism(pBuffDC, x, y, false, id));
					break;
				default:
					break;
			}
			
		}
		else
		{
			// The level:
			unsigned char* map = iniReader.ReadLargeString(sections, "map", "");

			memset(m_levelMap, 0x00, LEVEL_MAX_HEIGHT*LEVEL_MAX_WIDTH);

			int w = iniReader.ReadInteger(sections, "width", 0);
			int h = iniReader.ReadInteger(sections, "height", 0);


			for(int y=0; y<h; y++)
			{
				for(int x=0; x<w; x++)
				{

					m_levelMap[y][x] = *map-33;

					map++;
				}
			}
		}

		int k = strlen(sections);
		sections+=k+1;
	}
}

POINT Level::CheckDoor(CollisionPointGroup g)
{
	POINT returnPoint;
	returnPoint.x = -1;
	returnPoint.y = -1;

	vector<CollisionPoint>::iterator it;
	for(it = g.Begin(); it < g.End(); it++)
	{
		TileProperties tp = TileManager::Instance()->GetTileProperties(m_levelMap[(*it).y][(*it).x]);
		if(tp.id == 90)
		{
			returnPoint.x = (*it).x;
			returnPoint.y = (*it).y;
		}
	}
	return returnPoint;
}

void Level::RemoveDoor(int x, int y, unsigned char toID1, unsigned char toID2)
{
	m_levelMap[y][x] = toID1;
	m_levelMap[y+1][x] = toID2;
}

void Level::RemoveFromRepository(Enemy *e)
{
	worldRep.RemoveEnemyFromRepository(e);
}