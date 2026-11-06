#include "EnemyBase.h"
#include<DxLib.h>
#include"CommonState/EnemyIdleState.h"

EnemyBase::EnemyBase(void)
{
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::SubLoad(void)
{

}

void EnemyBase::SubInit(void)
{

}

void EnemyBase::SubUpdate(void)
{
	auto* curState = GetCurrentState();
	if (curState)
	{
		curState->IncreaseStateFrame();
	}
}

void EnemyBase::SubDraw(void)
{
}

void EnemyBase::SubRelease(void)
{
}
