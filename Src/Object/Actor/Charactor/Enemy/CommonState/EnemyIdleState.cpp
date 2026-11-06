#include "EnemyIdleState.h"
#include"../../../ActorBase.h"
#include"../../../../Common/AnimationController.h"
#include"../EnemyBase.h"
void EnemyIdleState::Enter(ActorBase* owner)
{
	isLoop_ = true;
	stateFrame_ = 0;
	auto* anim = owner->GetAnimationController();
	if (anim)
	{
		anim->Play((int)EnemyBase::ANIMAITON_TYPE::IDLE);

	}
}

void EnemyIdleState::Update(ActorBase* owner)
{
}

void EnemyIdleState::Exit(ActorBase* owner)
{
}
