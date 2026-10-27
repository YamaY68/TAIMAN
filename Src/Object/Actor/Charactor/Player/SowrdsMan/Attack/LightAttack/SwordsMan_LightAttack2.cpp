#include "SwordsMan_LightAttack2.h"
#include"../../../PlayerBase.h"
#include"../../../../../../../Manager/Input/KeyManager.h"
#include"../../../../../../Common/AnimationController.h"

#include"SwordsMan_LightAttack3.h"
#include"../HeavyAttack/SwordsMan_HeavyAttack3.h"
#include"../../../State/EvadeState.h"
#include"../../../State/IdleState.h"

void SwordsMan_LightAttack2::Enter(ActorBase* owner)
{
	isLoop_ = false;
	canChange_ = false;
	stateFrame_ = 0;
	totalAnimFrame_ = owner->GetAnimationController()->GetTotalTime();
	nextInputStartTime_ = totalAnimFrame_ * 0.4f;

	if (auto* anim = owner->GetAnimationController())
	{
		anim->Play((int)PlayerBase::ACTION_TYPE::LIGHT_ATTACK_2, false);
	}
}

void SwordsMan_LightAttack2::HandleInput(ActorBase* owner)
{
	if (!canChange_)return;
	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::LIGHTATTACK).down)
	{
		owner->ChangeState<SwordsMan_LightAttack3>();
	}
	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::HEAVYATTACK).down)
	{
		owner->ChangeState<SwordsMan_HeavyAttack3>();
	}
	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::EVADE).down)
	{
		owner->ChangeState<EvadeState>();
	}
}

void SwordsMan_LightAttack2::Update(ActorBase* owner)
{
	auto* anim = owner->GetAnimationController();
	if (!anim)
	{
		owner->ChangeState<IdleState>();
		return;
	}
	if (anim->GetProgressRate() >= 0.3f && anim->GetProgressRate() <= 0.5f)
	{
		auto f = owner->GetTransform().GetForward();
		owner->GetMoveMent().AddExternalPow(VScale(f, -2.5f));
		owner->GetMoveMent().AddExternalPow(VGet(0, 0.75, 0));
	}

	if (owner->GetAnimationController()->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_LightAttack2::Exit(ActorBase* owner)
{
	auto* player = static_cast<PlayerBase*>(owner);
	if (!player)return;
	player->SetNextComboChoices((int)PlayerBase::ACTION_TYPE::LIGHT_ATTACK_3, (int)PlayerBase::ACTION_TYPE::HEAVY_ATTACK_3);
	player->SetComboBuffer(15.0f);
}
