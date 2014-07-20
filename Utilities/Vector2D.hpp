#pragma once

#include <math.h>
#include "Globals.h"

class CVector2D
{
public:
	CVector2D(int p_x1, int p_y1, int p_x2, int p_y2)
	{
		x = p_x1;
		y = p_y1;
		newX = p_x2;
		newY = p_y2;
	}

	CVector2D(int p_x1, int p_y1)
	{
		x = p_x1;
		y = p_y1;
		newX = p_x1;
		newY = p_y1;
	}

	CVector2D(){}

	~CVector2D(void){}

	int X() const {return x;}
	int Y() const {return y;}
	int NewX() const {return newX;}
	int NewY() const {return newY;}
	int DX() const {return newX - x;}
	int DY() const {return newY - y;}

	void SetX(int p_x) {x = p_x;}
	void SetY(int p_y) {y = p_y;}
	void SetNewX(int p_x) {newX = p_x;}
	void SetNewY(int p_y) {newY = p_y;}
	void SetDX(int dx) {newX = x + dx;}
	void SetDY(int dy) {newY = y + dy;}
	void IncreaseDX(int dx) {newX = x + DX() + dx;}
	void IncreaseDY(int dy) {newY = y + DY() + dy;}

	double Length() const {return sqrt((double)(DX()*DX() + DY()*DY()));}

	void Step()
	{
		int dx = DX();
		int dy = DY();
		x = newX;
		y = newY;
		newX = x + dx;
		newY = y + dy;
	}

	bool CVector2D::operator==(const CVector2D &other) const
	{
		return (x == other.x) && (y == other.y) && (newX == other.newX) && (newY == other.newY);
	}

	bool CVector2D::operator!=(const CVector2D &other) const
	{
		return !(*this == other);
	}

	int DirectionAsConstants() const
	{
		int r = COLL_NONE;
		r |= COLL_TOP * (newY < y);
		r |= COLL_BOTTOM * (newY > y);
		r |= COLL_LEFT * (newX < x);
		r |= COLL_RIGHT* (newX > x);
		return r;
	}

private:
	int x;
	int y;
	int newX;
	int newY;
};