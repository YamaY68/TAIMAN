#include "SwordsMan_HeavyAttack4.h"
#include"../../../PlayerBase.h"
#include"../../../../../../../Manager/Input/KeyManager.h"
#include"../../../../../../Common/AnimationController.h"
#include"../../../State/IdleState.h"
void SwordsMan_HeavyAttack4::Enter(ActorBase* owner)
{
	isLoop_ = false;
	canChange_ = false;
	stateFrame_ = 0;
	totalAnimFrame_ = owner->GetAnimationController()->GetTotalTime();
	owner->GetAnimationController()->Play((int)PlayerBase::ACTION_TYPE::HEAVY_ATTACK_4, false);
	auto* player = static_cast<PlayerBase*>(owner);
	auto dir = player->GetIntendInputMoveDir();
	player->GetTransform().quaRot = Quaternion::Euler(VGet(0.0f, atan2f(dir.x, dir.z), 0.0f));

}


void SwordsMan_HeavyAttack4::HandleInput(ActorBase* owner)
{
}

void SwordsMan_HeavyAttack4::Update(ActorBase* owner)
{
	auto* anim = owner->GetAnimationController();
	if (!anim)
	{
		owner->ChangeState<IdleState>();
		return;
	}
	if (anim->GetProgressRate() >= 0.28f && anim->GetProgressRate() <= 0.3f)
	{
		auto f = owner->GetTransform().GetForward();
		owner->GetMoveMent().AddExternalPow(VScale(f, 6.0f));
	}
	else if (anim->GetProgressRate() >= 0.32f && anim->GetProgressRate() <= 0.34f)
	{
		owner->GetMoveMent().SetExternalPow(VGet(0, 0, 0));
	}
	else if (anim->GetProgressRate() >= 0.38 && anim->GetProgressRate() <= 0.4f)
	{
		owner->GetMoveMent().AddExternalPow(VScale(owner->GetTransform().GetForward(), 6.0f));
	}
	else if (anim->GetProgressRate() >= 0.5 && anim->GetProgressRate() <= 0.51f)
	{
		owner->GetMoveMent().AddExternalPow(VGet(0, 1.5f, 0));
	}
	else if (anim->GetProgressRate() >= 0.52f && anim->GetProgressRate() <= 0.53f)
	{
		auto f = owner->GetTransform().GetForward();
		owner->GetMoveMent().AddExternalPow(VScale(f, 1.0f));
	}

	if (owner->GetAnimationController()->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_HeavyAttack4::Exit(ActorBase* owner)
{
}
