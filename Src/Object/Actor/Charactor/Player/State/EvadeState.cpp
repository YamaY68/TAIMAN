#include "EvadeState.h"
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"
#include"../../../../../Utility/AsoUtility.h"
#include"../../../Camera/CameraManager.h"

void EvadeState::Enter(ActorBase* owner)
{
	isLoop_ = false;

	auto* anim = owner->GetAnimationController();
	auto* player = dynamic_cast<PlayerBase*>(owner);
	if (!anim || !player) return;

	// 回避方向とプレイヤー前方向の角度が一致しない場合は通常回避
	if (VDot(player->GetTransform().GetForward(), evadeDirection_) < 0.99f)
	{
		anim->Play((int)PlayerBase::ANIMATION_TYPE::EVADE, false);
		anim->SetBlendAnim((int)PlayerBase::ANIMATION_TYPE::IDLE, 0.5f, true);
		return;
	}

	// 前方向回避時カメラ方向との比較
	auto* cameraManager = player->GetCameraManager();
	auto f = cameraManager ? cameraManager->GetCameraForwardXZ() : player->GetTransform().GetForward();

	if (VDot(f, evadeDirection_) >= 0.99f)
	{
		// バックステップへ変化
		anim->Play((int)PlayerBase::ANIMATION_TYPE::EVADE, false);
		anim->SetBlendAnim((int)PlayerBase::ANIMATION_TYPE::IDLE, 0.5f, true);
		evadeDirection_ = VScale(f, -1.0f);
	}
	else
	{
		// ダイブへ変化
		anim->Play((int)PlayerBase::ANIMATION_TYPE::DIVE, false);
		isDive_ = true;
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
		anim->RemoveBlendAnim((int)PlayerBase::ANIMATION_TYPE::IDLE);
	}
}