#include "SwordsMan_HeavyAttack1.h"
#include"../../../PlayerBase.h"
#include"../../../../../../../Manager/Input/KeyManager.h"
#include"../../../../../../Common/AnimationController.h"
#include"../../../State/IdleState.h"

void SwordsMan_HeavyAttack1::Enter(ActorBase* owner)
{
	isLoop_ = false;
	canChange_ = false;
	stateFrame_ = 0;
	totalAnimFrame_ = owner->GetAnimationController()->GetTotalTime();
	owner->GetAnimationController()->Play((int)PlayerBase::ACTION_TYPE::HEAVY_ATTACK_1, false);
}

void SwordsMan_HeavyAttack1::HandleInput(ActorBase* owner)
{
}

void SwordsMan_HeavyAttack1::Update(ActorBase* owner)
{
	auto* anim = owner->GetAnimationController();
	if (!anim)
	{
		owner->ChangeState<IdleState>();
	}
	if (anim->GetProgressRate() >= 0.22f && anim->GetProgressRate() <= 0.3f)
	{
		auto f = owner->GetTransform().GetForward();
		owner->GetMoveMent().AddExternalPow(VScale(f, -1.0f));
	}

	if (owner->GetAnimationController()->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_HeavyAttack1::Exit(ActorBase* owner)
{
}
