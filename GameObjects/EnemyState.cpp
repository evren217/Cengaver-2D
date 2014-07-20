#include "EnemyState.h"
#include "../Utilities/Macro.h"
#include "State.h"

EnemyState::EnemyState(void)
{
	name = "None\0";
}

EnemyState::~EnemyState(void)
{
}

void EnemyState::SeeHero(Enemy *e){}
void EnemyState::LoseHero(Enemy *e){}
void EnemyState::EnterViewpoint(Enemy *e){}
void EnemyState::ExitViewpoint(Enemy *e)
{
	e->SetCurrentState(new WaitingEnemy());
}
void EnemyState::Execute(Enemy* e){}
void EnemyState::Exit(Enemy* e){}

WaitingEnemy::WaitingEnemy(void)
{
	name = "Waiting\0";
}
void WaitingEnemy::EnterViewpoint(Enemy *e)
{
	e->SetStartingState();
}
void WaitingEnemy::Execute(Enemy* e){}
void WaitingEnemy::Exit(Enemy* e){}

PatrollingEnemy::PatrollingEnemy(void)
{
	name = "Patrolling\0";
}
void PatrollingEnemy::Execute(Enemy* e){
	e->ai->EnemyPatrol(e);
}
void PatrollingEnemy::Exit(Enemy* e){
	e->ai->EnemyStop(e);
}
void PatrollingEnemy::ExitViewpoint(Enemy *e)
{
	e->SetCurrentState(new WaitingEnemy());
}

HoppingEnemy::HoppingEnemy(void)
{
	name = "Hopping\0";
}
void HoppingEnemy::Execute(Enemy* e)
{
	e->ai->EnemyHopping(e);
}
void HoppingEnemy::Exit(Enemy* e)
{
	e->ai->EnemyStop(e);
}
void HoppingEnemy::ExitViewpoint(Enemy *e)
{
	e->SetCurrentState(new WaitingEnemy());
}

DyingEnemy::DyingEnemy()
{
	name = "Dying";
}
void DyingEnemy::Execute(Enemy* e)
{
	Exit(e);
}
void DyingEnemy::Exit(Enemy* e)
{
	e->ai->RemoveFromRepository(e);
}

RammingEnemy::RammingEnemy(void)
{
	name = "Ramming\0";
}
void RammingEnemy::Execute(Enemy* e)
{
	e->ai->EnemyRam(e);
}
void RammingEnemy::Exit(Enemy* e)
{
	e->ai->EnemyStop(e);
}
void RammingEnemy::ExitViewpoint(Enemy *e)
{
	e->SetCurrentState(new WaitingEnemy());
}

BossEnemy::BossEnemy(void)
{
	name = "Boss\0";
}
void BossEnemy::Execute(Enemy* e)
{
	e->ai->EnemyBoss(e);
}
void BossEnemy::Exit(Enemy* e)
{
	e->ai->EnemyStop(e);
}
void BossEnemy::ExitViewpoint(Enemy *e)
{
	e->SetCurrentState(new WaitingEnemy());
}