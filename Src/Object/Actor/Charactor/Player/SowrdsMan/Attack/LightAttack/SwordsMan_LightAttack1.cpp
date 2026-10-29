#include "SwordsMan_LightAttack1.h"
#include"../../../PlayerBase.h"
#include"../../../../../../../Manager/Input/KeyManager.h"
#include"../../../../../../Common/AnimationController.h"
#include"../../../../../Camera/CameraManager.h"

#include"SwordsMan_LightAttack2.h"
#include"../HeavyAttack/SwordsMan_HeavyAttack2.h"
#include"../../../State/EvadeState.h"
#include"../../../State/IdleState.h"

void SwordsMan_LightAttack1::Enter(ActorBase* owner)
{
	isLoop_ = false;
	canChange_ = false;
	stateFrame_ = 0;
	totalAnimFrame_ = owner->GetAnimationController()->GetTotalTime();
	nextInputStartTime_ = totalAnimFrame_ * 0.4f;

	if (auto* anim = owner->GetAnimationController())
	{
		anim->Play((int)PlayerBase::ACTION_TYPE::LIGHT_ATTACK_1, false);
	}
	auto* player = static_cast<PlayerBase*>(owner);
	auto dir = player->GetIntendInputMoveDir();
	player->GetTransform().quaRot = Quaternion::Euler(VGet(0.0f, atan2f(dir.x, dir.z), 0.0f));
}

void SwordsMan_LightAttack1::HandleInput(ActorBase* owner)
{
	if (!canChange_)return;
	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::LIGHTATTACK).down)
	{
		owner->ChangeState<SwordsMan_LightAttack2>();
	}
	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::HEAVYATTACK).down)
	{
		owner->ChangeState<SwordsMan_HeavyAttack2>();
	}
	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::EVADE).down)
	{
		auto* player = static_cast<PlayerBase*>(owner);
		auto dir = player->GetIntendInputMoveDir();
		auto* evade = player->GetState<EvadeState>();
		if (evade)evade->SetEvadeDirection(dir);
		owner->ChangeState<EvadeState>();
	}
}

void SwordsMan_LightAttack1::Update(ActorBase* owner)
{
	auto* anim = owner->GetAnimationController();
	if (!anim)
	{
		owner->ChangeState<IdleState>();
	}
	if (anim->GetProgressRate() >= 0.1f && anim->GetProgressRate() <= 0.2f)
	{
		auto f = owner->GetTransform().GetForward();
		owner->GetMoveMent().AddExternalPow(VScale(f, 2.0f));
		owner->GetMoveMent().AddExternalPow(VGet(0, 0.5f, 0));
	}

	if (owner->GetAnimationController()->IsEnd())
	{
		owner->ChangeState<IdleState>();
	}
}

void SwordsMan_LightAttack1::Exit(ActorBase* owner)
{
	auto* player = static_cast<PlayerBase*>(owner);
	if (!player)return;
	player->SetNextComboChoices((int)PlayerBase::ACTION_TYPE::LIGHT_ATTACK_2, (int)PlayerBase::ACTION_TYPE::HEAVY_ATTACK_2);
	player->SetComboBuffer(15.0f);
}
