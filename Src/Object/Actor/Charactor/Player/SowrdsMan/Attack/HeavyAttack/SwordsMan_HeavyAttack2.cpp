#include "SwordsMan_HeavyAttack2.h"
#include"../../../PlayerBase.h"
#include"../../../../../../../Manager/Input/KeyManager.h"
#include"../../../../../../Common/AnimationController.h"
#include"../../../State/IdleState.h"

void SwordsMan_HeavyAttack2::Enter(ActorBase* owner)
{
	isLoop_ = false;
	canChange_ = false;
	stateFrame_ = 0;
	totalAnimFrame_ = owner->GetAnimationController()->GetTotalTime();
	owner->GetAnimationController()->Play((int)PlayerBase::ACTION_TYPE::HEAVY_ATTACK_2, false);
	auto* player = static_cast<PlayerBase*>(owner);
	auto dir = player->GetIntendInputMoveDir();
	player->GetTransform().quaRot = Quaternion::Euler(VGet(0.0f, atan2f(dir.x, dir.z), 0.0f));

}

void SwordsMan_HeavyAttack2::HandleInput(ActorBase* owner)
{
}

void SwordsMan_HeavyAttack2::Update(ActorBase* owner)
{
	auto* anim = owner->GetAnimationController();
	if (!anim)
	{
		owner->ChangeState<IdleState>();
		return;
	}
	if (anim->GetProgressRate() >= 0.2f && anim->GetProgressRate() <= 0.22f)
	{
		auto f = owner->GetTransform().GetForward();
		owner->GetMoveMent().AddExternalPow(VScale(f, 12.0f));
	}
	else if (anim->GetProgressRate() >= 0.3f&&anim->GetProgressRate()<=0.4f)
	{
		owner->GetMoveMent().SetExternalPow(VGet(0, 0, 0));
	}
	else if (anim->GetProgressRate() >= 0.5 && anim->GetProgressRate() <= 0.52f)
	{
		owner->GetMoveMent().AddExternalPow(VScale(owner->GetTransform().GetForward(), 10.5f));
	}
	if (owner->GetAnimationController()->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_HeavyAttack2::Exit(ActorBase* owner)
{
}
