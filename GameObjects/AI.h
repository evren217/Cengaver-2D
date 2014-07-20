#pragma once
#include "../Utilities/Vector2D.hpp"
#include "../Utilities/CollisionPoint.hpp"

class Level;
class Enemy;
class CMovableObject;

class AI
{
	public:
		AI();
		AI(Level* level_p);
		~AI(void);
		
		CollisionPointGroup CheckCollision(CMovableObject* object) const;
		bool CheckCollisionWithEnemies(CMovableObject* object) const;
		bool WillFall(CMovableObject* object, int steps) const;
		void SetLevel(Level* level_p);
		void EnemyPatrol(CMovableObject* object);
		void EnemyHopping(CMovableObject* object);
		void EnemyStop(CMovableObject* object);
		void EnemyRam(CMovableObject* object);
		void EnemyBoss(CMovableObject* object); 

		bool BoundingBoxCollision(CMovableObject* o, int left2, int right2, int top2, int bottom2) const;
		void RemoveFromRepository(Enemy* e);

		Level* level;

	private:
		UCHAR counter;
		UCHAR offset;
		int prevDX;
		bool genBool;
		bool switchBool;
};