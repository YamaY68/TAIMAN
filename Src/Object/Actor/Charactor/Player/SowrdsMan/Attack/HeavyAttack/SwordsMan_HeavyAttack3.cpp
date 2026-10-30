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
	owner->GetAnimationController()->Play((int)PlayerBase::ANIMATION_TYPE::HEAVY_ATTACK_3, false);
	auto* player = static_cast<PlayerBase*>(owner);
	auto dir = player->GetIntendInputMoveDir();
	player->GetTransform().quaRot = Quaternion::Euler(VGet(0.0f, atan2f(dir.x, dir.z), 0.0f));

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

	const float rate = anim->GetProgressRate();
	const VECTOR forward = owner->GetTransform().GetForward();

	if (rate >= 0.28f && rate <= 0.3f)
	{
		owner->GetMoveMent().AddExternalPow(VScale(forward, 6.0f));
	}
	else if (rate >= 0.32f && rate <= 0.34f)
	{
		owner->GetMoveMent().SetExternalPow(VGet(0.0f, 0.0f, 0.0f));
	}
	else if (rate >= 0.38f && rate <= 0.4f)
	{
		owner->GetMoveMent().AddExternalPow(VScale(forward, 4.0f));
	}
	else if (rate >= 0.5f && rate <= 0.51f)
	{
		owner->GetMoveMent().AddExternalPow(VGet(0.0f, 1.5f, 0.0f));
	}
	else if (rate >= 0.52f && rate <= 0.53f)
	{
		owner->GetMoveMent().AddExternalPow(VScale(forward, 1.0f));
	}

	if (anim->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_HeavyAttack3::Exit(ActorBase* owner)
{
}
