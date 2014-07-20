#pragma once
#include <Vector>
#include "Gadget.h"
#include "Enemy.h"
#include "../UserInterface/Viewpoint.h"

using namespace std;

class WorldRepository
{
public:
	WorldRepository(void);
	~WorldRepository(void);

	unsigned short GetEnemyCount();
	unsigned short GetGadgetCount();

	void AddEnemy(Enemy* e);
	void AddGadget(Gadget* g);

	Gadget* GetGadgetAtPos(CMovableObject* object);
	Enemy* GetEnemyAtPos(int x, int y);

	Gadget* RemoveGadgetFromRepository(Gadget* g);
	Enemy* RemoveEnemyFromRepository(Enemy* e);

	void DrawAllGadgetsInRepository(Viewpoint window);
	void DrawAllEnemiesInRepository(Viewpoint window);
	void UpdateEnemies(Viewpoint window);

	vector<Gadget*> gadgets;
	vector<Enemy*> enemies;

private:
	unsigned short gadgetCount;
	unsigned short enemyCount;
};