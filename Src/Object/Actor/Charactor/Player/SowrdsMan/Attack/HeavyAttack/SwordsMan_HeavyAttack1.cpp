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

	auto* anim = owner->GetAnimationController();
	if (anim)
	{
		totalAnimFrame_ = static_cast<int>(anim->GetTotalTime());
		anim->Play(static_cast<int>(PlayerBase::ANIMATION_TYPE::HEAVY_ATTACK_1), false);
	}

	auto* player = static_cast<PlayerBase*>(owner);
	const VECTOR dir = player->GetIntendInputMoveDir();
	if (VSquareSize(dir) > 0.0001f)
	{
		player->GetTransform().quaRot = Quaternion::Euler(VGet(0.0f, atan2f(dir.x, dir.z), 0.0f));
	}
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
		return;
	}

	const float rate = anim->GetProgressRate();
	if (rate >= 0.22f && rate <= 0.3f)
	{
		const VECTOR forward = owner->GetTransform().GetForward();
		owner->GetMoveMent().AddExternalPow(VScale(forward, -1.0f));
	}

	if (anim->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_HeavyAttack1::Exit(ActorBase* owner)
{
}
