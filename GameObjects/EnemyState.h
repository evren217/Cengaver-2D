#pragma once

#include "Enemy.h"
#include "State.h"

class Enemy;

class EnemyState : public State
{
public:
	EnemyState(void);
	~EnemyState(void);

	virtual void SeeHero(Enemy* e);
	virtual void LoseHero(Enemy* e);
	virtual void EnterViewpoint(Enemy* e);
	virtual void ExitViewpoint(Enemy* e);
	virtual void Execute(Enemy* e);
	virtual void Exit(Enemy* e);
};

class WaitingEnemy : public EnemyState
{
public:
	WaitingEnemy(void);
	void Exit(Enemy* e);
	void Execute(Enemy* e);
	void EnterViewpoint(Enemy* e);
};

class PatrollingEnemy : public EnemyState
{
public:
	PatrollingEnemy(void);
	void Execute(Enemy* e);
	void Exit(Enemy* e);
	void ExitViewpoint(Enemy* e);
};

class HoppingEnemy : public EnemyState
{
public:
	HoppingEnemy(void);
	void Execute(Enemy* e);
	void Exit(Enemy* e);
	void ExitViewpoint(Enemy* e);
};

class RammingEnemy : public EnemyState
{
public:
	RammingEnemy(void);
	void Execute(Enemy* e);
	void Exit(Enemy* e);
	void ExitViewpoint(Enemy* e);
};

class BossEnemy : public EnemyState
{
public:
	BossEnemy(void);
	void Execute(Enemy* e);
	void Exit(Enemy* e);
	void ExitViewpoint(Enemy* e);
};

class DyingEnemy : public EnemyState
{
public:
	DyingEnemy(void);
	void Execute(Enemy* e);
	void Exit(Enemy* e);
};