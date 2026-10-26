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
}

void SwordsMan_FinishAttack::HandleInput(ActorBase* owner)
{
}

void SwordsMan_FinishAttack::Update(ActorBase* owner)
{
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
