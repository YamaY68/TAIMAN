#include "EvadeState.h"
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"
#include"../../../../../Utility/AsoUtility.h"
#include"../../../Camera/CameraManager.h"

void EvadeState::Enter(ActorBase* owner)
{
	isLoop_ = false;
	nextInputStartTime_ = 60;
	if (auto* anim = owner->GetAnimationController())
	{
		auto* player = dynamic_cast<PlayerBase*>(owner);
		if (VDot(player->GetTransform().GetForward(), evadeDirection_) >= 0.99)
		{
			auto* cameramanager = player->GetCameraManager();
			auto f=cameramanager->GetCameraForwardXZ();
			if (VDot(f, evadeDirection_) >= 0.99)
			{
				anim->Play((int)PlayerBase::ACTION_TYPE::EVADE, false);
				anim->SetBlendAnim((int)PlayerBase::ACTION_TYPE::IDLE, 0.5f, true);
				evadeDirection_ = VScale(f, -1);
			}
			else
			{
			anim->Play((int)PlayerBase::ACTION_TYPE::DIVE, false);
			isDive_ = true;
			}
		}
		else
		{
			anim->Play((int)PlayerBase::ACTION_TYPE::EVADE, false);
			anim->SetBlendAnim((int)PlayerBase::ACTION_TYPE::IDLE, 0.5f, true);
		}
	}
}

void EvadeState::HandleInput(ActorBase* owner)
{
}

void EvadeState::Update(ActorBase* owner)
{
	auto& pos = owner->GetTransform().pos;
	auto& movement = owner->GetMoveMent();
	VECTOR forward = owner->GetTransform().GetForward();
	if (VSquareSize(evadeDirection_) > 0.0001f)
	{
		if (isDive_)
		{
			movement.SetInputMovePow(VScale(forward, 4.8f));
		}
		else
		{
			movement.SetInputMovePow(VScale(evadeDirection_, 4.8f));
			//pos = VAdd(pos, VScale(evadeDirection_, 3.8f));
		}
	}
	else
	{
		movement.SetInputMovePow(VScale(forward, 3.8f));
		//pos = VAdd(pos, VScale(forward, 3.8f));
	}

	if (auto* anim = owner->GetAnimationController())
	{
		if (anim->IsEnd())
		{
			owner->ChangeState<IdleState>();
		}
	}
}

void EvadeState::Exit(ActorBase* owner)
{
	stateFrame_ = 0;
	evadeDirection_ = { 0.0f, 0.0f, 0.0f };
	isDive_ = false;
	if (auto* anim = owner->GetAnimationController())
	{
		if (isDive_)return;
		anim->RemoveBlendAnim((int)PlayerBase::ACTION_TYPE::IDLE);
	}
}