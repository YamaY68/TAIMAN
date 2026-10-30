#include "SwordsMan_FinishAttack.h"

#include"../../PlayerBase.h"
#include"../../../../../../Manager/Input/KeyManager.h"
#include"../../../../../Common/AnimationController.h"

#include"../../State/EvadeState.h"
#include"../../State/IdleState.h"

void SwordsMan_FinishAttack::Enter(ActorBase* owner)
{
	isLoop_ = false;
	canChange_ = false;
	stateFrame_ = 0;
	totalAnimFrame_ = owner->GetAnimationController()->GetTotalTime();
	if (auto* anim = owner->GetAnimationController())
	{
		anim->Play((int)PlayerBase::ANIMATION_TYPE::FINISH, false);
	}
	owner->GetMoveMent().AddExternalPow(VGet(0, 3, 0));
	auto* player = static_cast<PlayerBase*>(owner);
	auto dir = player->GetIntendInputMoveDir();
	player->GetTransform().quaRot = Quaternion::Euler(VGet(0.0f, atan2f(dir.x, dir.z), 0.0f));

}

void SwordsMan_FinishAttack::HandleInput(ActorBase* owner)
{
}

void SwordsMan_FinishAttack::Update(ActorBase* owner)
{
	auto* anim = owner->GetAnimationController();
	if (!anim)
	{
		owner->ChangeState<IdleState>();
		return;
	}

	const float rate = anim->GetProgressRate();
	const VECTOR forward = owner->GetTransform().GetForward();

	if (rate >= 0.0f && rate <= 0.1f)
	{
		owner->GetMoveMent().AddExternalPow(VScale(forward, -1.5f));
	}
	else if (rate >= 0.25f && rate <= 0.3f)
	{
		owner->GetMoveMent().AddExternalPow(VScale(forward, 1.5f));
	}

	if (anim->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_FinishAttack::Exit(ActorBase* owner)
{
	auto* player = static_cast<PlayerBase*>(owner);
	if (!player)return;
	player->SetNextComboChoices(PlayerBase::ATTACK_TYPE::LIGHT_ATTACK_1, PlayerBase::ATTACK_TYPE::HEAVY_ATTACK_1);
	player->SetComboBuffer(15.0f);
}
