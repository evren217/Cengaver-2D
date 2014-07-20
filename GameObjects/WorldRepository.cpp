#include "WorldRepository.h"
#include "../Utilities/Macro.h"
#include "../Utilities/Globals.h"

class CMovableObject;

WorldRepository::WorldRepository(void)
{
	gadgetCount = 0;
	enemyCount = 0;
}

WorldRepository::~WorldRepository(void)
{
	for(int i=0;i<gadgets.size();i++)
		CLEAN(gadgets[i]);
	for(int i=0;i<enemies.size();i++)
		CLEAN(enemies[i]);
}

void WorldRepository::AddGadget(Gadget *g)
{
	vector<Gadget*>::iterator it;
	for(it = gadgets.begin();it < gadgets.end();it++)
	{
		if((*it) == 0)
		{
			gadgets.insert(it, g);
			gadgetCount++;
			return;
		}
	}
	gadgets.push_back(g);
	gadgetCount++;
}

void WorldRepository::AddEnemy(Enemy *e)
{
	vector<Enemy*>::iterator it;
	for(it = enemies.begin();it < enemies.end();it++)
	{
		if((*it) == 0)
		{
			enemies.insert(it, e);
			enemyCount++;
			return;
		}
	}
	enemies.push_back(e);
	enemyCount++;
}

void WorldRepository::DrawAllGadgetsInRepository(Viewpoint window)
{
	for(int i=0;i<gadgets.size();i++)
		if(gadgets[i])
			gadgets[i]->Draw(window);
}

void WorldRepository::DrawAllEnemiesInRepository(Viewpoint window)
{
	for(int i=0;i<enemies.size();i++)
		if(enemies[i])
			enemies[i]->Draw(window);
}

Gadget* WorldRepository::RemoveGadgetFromRepository(Gadget *g)
{
	vector<Gadget*>::iterator it;
	for(it = gadgets.begin();it < gadgets.end();it++)
	{
		if((*it) == g)
		{
			gadgets.erase(it);
			return g;
		}
	}
	return 0;
}

Enemy* WorldRepository::RemoveEnemyFromRepository(Enemy *e)
{
	vector<Enemy*>::iterator it;
	for(it = enemies.begin();it < enemies.end();it++)
	{
		if((*it) == e)
		{
			(*it) = 0;
			return e;
		}
	}
	return 0;
}

Gadget* WorldRepository::GetGadgetAtPos(CMovableObject* object)
{
	for (int i = 0; i < gadgets.size(); i++)
	{
		if (gadgets[i] != 0)
		{
			bool coll = object->ai->BoundingBoxCollision(object, gadgets[i]->xPos, gadgets[i]->xPos + TILESIZE, gadgets[i]->yPos, gadgets[i]->yPos + TILESIZE);

			if (coll)
			{
				return gadgets[i];
			}
		}
	}

	return 0;
}

void WorldRepository::UpdateEnemies(Viewpoint window){
	vector<Enemy*>::iterator it;
	vector<int> emptyIndices;
	
	for(it = enemies.begin() ;it != enemies.end();it++)
	{
		
		if((*it) != NULL)
			if((*it)->movementVector.X() > window.viewX-100 && 
				(*it)->movementVector.X() < window.viewX+window.width+100 &&
				(*it)->movementVector.Y() > window.viewY-100 &&
				(*it)->movementVector.Y() < window.viewY+window.height + 100)
					(*it)->Move(0,0,0);
		
		if((*it) == NULL)
			emptyIndices.push_back(it-enemies.begin());
	}

	vector<int>::reverse_iterator  it2;
	for(it2 = emptyIndices.rbegin();it2 != emptyIndices.rend();it2++)
	{
		enemies.erase(enemies.begin() + (*it2));
	}
}