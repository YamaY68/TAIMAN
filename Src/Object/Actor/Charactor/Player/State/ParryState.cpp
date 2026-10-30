#include "ParryState.h"
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"

void ParryState::Enter(ActorBase* owner)
{
    isLoop_ = false;
    if (auto* anim = owner->GetAnimationController())
    {
        anim->Play((int)PlayerBase::ANIMATION_TYPE::PARRY, false);
    }
}

void ParryState::HandleInput(ActorBase* owner)
{
}

void ParryState::Update(ActorBase* owner)
{
    VECTOR forward = owner->GetTransform().GetForward();
    owner->GetMoveMent().SetInputMovePow(VScale(forward, 0.2f));

    if (auto* anim = owner->GetAnimationController())
    {
        if (anim->IsEnd())
        {
            owner->ChangeState<IdleState>();
        }
    }
}

void ParryState::Exit(ActorBase* owner)
{
    stateFrame_ = 0;

}