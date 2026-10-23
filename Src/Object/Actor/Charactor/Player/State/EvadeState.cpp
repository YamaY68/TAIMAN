#include "EvadeState.h"
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"

void EvadeState::Enter(ActorBase* owner)
{
    isLoop_ = false;
    nextInputStartTime_ = 60;

    if (auto* anim = owner->GetAnimationController())
    {
        anim->Play((int)PlayerBase::ANIM_TYPE::EVADE, false);
    }
}

void EvadeState::HandleInput(ActorBase* owner)
{
}

void EvadeState::Update(ActorBase* owner)
{
    auto& pos = owner->GetTransform().pos;

    if (VSquareSize(evadeDirection_) > 0.0001f)
    {
        pos = VAdd(pos, VScale(evadeDirection_, 3.8f));
    }
    else
    {
        VECTOR forward = owner->GetTransform().GetForward();
        pos = VAdd(pos, VScale(forward, 3.8f));
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
    evadeDirection_ = { 0.0f, 0.0f, 0.0f };
}