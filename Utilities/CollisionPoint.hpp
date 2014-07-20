#pragma once

#include <Windows.h>
#include <vector>
#include <algorithm>

class CollisionPoint
{
public:
	int x;
	int y;
	double pseudoDistance;
	unsigned char collisionDirection;
	bool isEnemy;

	CollisionPoint()
	{
		x = 0;
		y = 0;
		pseudoDistance = 0;
		collisionDirection = 0;
		isEnemy = false;
	}

	bool CollisionPoint::operator==( CollisionPoint &other )
	{
		return x == other.x && y == other.y && pseudoDistance == other.pseudoDistance;
	}
};

class CollisionPointGroup
{
private:
	std::vector<CollisionPoint> points;

public:
	void Add(const CollisionPoint cp)
	{ 
		points.push_back(cp);
	}
	void Add(const int x, const int y, const unsigned char collDir)
	{ 
		CollisionPoint cp; 
		cp.x = x; 
		cp.y = y; 
		cp.pseudoDistance = -1; 
		cp.collisionDirection = collDir; 
		points.push_back(cp);
	}
	void AddByMovingX(int byPixels) 
	{ 
		std::vector<CollisionPoint>::iterator it; 
		for(it = points.begin(); it != points.end(); it++)
		{
			CollisionPoint cp = CollisionPoint(*it); 
			cp.x = (*it).x + byPixels;
			points.push_back(cp);
		} 
	}
	void AddByMovingY(int byPixels) 
	{ 
		std::vector<CollisionPoint>::iterator it; 
		for(it = points.begin(); it != points.end(); it++)
		{
			CollisionPoint cp = CollisionPoint(*it); 
			cp.y = (*it).y + byPixels; 
			points.push_back(cp);
		} 
	}
	void AddByMoving(int xByTiles, int yByTiles, int times) 
	{
		std::vector<CollisionPoint>::iterator it;
		std::vector<CollisionPoint> buffer = std::vector<CollisionPoint>();
		buffer.resize(points.size()*times);
		for(int i = 1; i<= times;i++)
		{
			for(it = points.begin(); it != points.end(); it++) 
			{
				CollisionPoint cp = CollisionPoint(*it);
				cp.x = (*it).x + xByTiles * i;
				cp.y = (*it).y + yByTiles * i;
				buffer.push_back(cp);
			}
		}
		for(it = buffer.begin(); it != buffer.end(); it++) 
		{
			points.push_back((*it));
		}
		points.erase(std::unique(points.begin(), points.end()), points.end());
	}
	void CopyTo(std::vector<CollisionPoint>* collection) 
	{ 
		std::vector<CollisionPoint>::iterator it; 
		for(it = points.begin(); it != points.end(); it++)
		{
			collection->push_back(*it);
		} 
	}
	std::vector<CollisionPoint>::iterator Begin() 
	{ 
		return points.begin(); 
	}
	std::vector<CollisionPoint>::iterator End() 
	{ 
		return points.end(); 
	}
	int Size()
	{
		return points.size();
	}
};