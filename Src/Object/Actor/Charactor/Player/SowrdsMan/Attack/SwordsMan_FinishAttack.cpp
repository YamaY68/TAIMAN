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
		anim->Play((int)PlayerBase::ACTION_TYPE::FINISH, false);
	}
	owner->GetMoveMent().AddExternalPow(VGet(0, 3, 0));
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
	if (anim->GetProgressRate() >= 0.0f && anim->GetProgressRate() <= 0.1f)
	{
		auto f = owner->GetTransform().GetForward();
		owner->GetMoveMent().AddExternalPow(VScale(f, -1.5f));
	}
	else if (anim->GetProgressRate() >= 0.25 && anim->GetProgressRate() <= 0.3f)
	{
		owner->GetMoveMent().AddExternalPow(VScale(owner->GetTransform().GetForward(), 1.5));
	}

	if (owner->GetAnimationController()->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_FinishAttack::Exit(ActorBase* owner)
{
	auto* player = static_cast<PlayerBase*>(owner);
	if (!player)return;
	player->SetNextComboChoices((int)PlayerBase::ACTION_TYPE::LIGHT_ATTACK_1, (int)PlayerBase::ACTION_TYPE::HEAVY_ATTACK_1);
	player->SetComboBuffer(15.0f);
}
