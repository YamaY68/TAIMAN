#include "SwordsMan_HeavyAttack3.h"
#include"../../../PlayerBase.h"
#include"../../../../../../../Manager/Input/KeyManager.h"
#include"../../../../../../Common/AnimationController.h"
#include"../../../State/IdleState.h"
void SwordsMan_HeavyAttack3::Enter(ActorBase* owner)
{
	isLoop_ = false;
	canChange_ = false;
	stateFrame_ = 0;
	totalAnimFrame_ = owner->GetAnimationController()->GetTotalTime();
	owner->GetAnimationController()->Play((int)PlayerBase::ACTION_TYPE::HEAVY_ATTACK_3, false);
}

void SwordsMan_HeavyAttack3::HandleInput(ActorBase* owner)
{
}

void SwordsMan_HeavyAttack3::Update(ActorBase* owner)
{
	auto* anim = owner->GetAnimationController();
	if (!anim)
	{
		owner->ChangeState<IdleState>();
		return;
	}

	if (anim->GetProgressRate() >= 0.25f && anim->GetProgressRate() <= 0.3f)
	{
		auto f = owner->GetTransform().GetForward();
		owner->GetMoveMent().AddExternalPow(VScale(f, 2.0f));
	}
	else if (anim->GetProgressRate() >= 0.48f && anim->GetProgressRate() <= 0.53f)
	{
		owner->GetMoveMent().AddExternalPow(VScale(owner->GetTransform().GetForward(), -3.50f));
	}
	
	if (owner->GetAnimationController()->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_HeavyAttack3::Exit(ActorBase* owner)
{
}
