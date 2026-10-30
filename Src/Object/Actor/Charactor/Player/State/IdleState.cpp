#include "IdleState.h"
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"
#include "../../../../../Manager/Input/KeyManager.h"

void IdleState::Enter(ActorBase* owner)
{
    isLoop_ = true;
    if (auto* anim = owner->GetAnimationController())
    {
        anim->Play((int)PlayerBase::ANIMATION_TYPE::IDLE);
    }
}

void IdleState::HandleInput(ActorBase* owner)
{
    auto* player = static_cast<PlayerBase*>(owner);
    auto& key = KeyManager::GetInstance();

    // ‰ñ”ð“ü—Íi‘O•û‚Ì‹tƒxƒNƒgƒ‹‚ðÝ’è‚µ‚Ä‘JˆÚj
    if (key.GetInfo(KEY::KEY_TYPE::EVADE).down)
    {
        auto* evade = player->GetState<EvadeState>();
        const VECTOR f = player->GetTransform().GetForward();
        if (evade) evade->SetEvadeDirection(VScale(f, -1.0f));
        player->ChangeState<EvadeState>();
        return;
    }

    // ƒK[ƒh“ü—Í
    if (key.GetInfo(KEY::KEY_TYPE::GUARD).now)
    {
        player->ChangeState<GuardState>();
        return;
    }

    // ŽãUŒ‚“ü—Í
    if (key.GetInfo(KEY::KEY_TYPE::LIGHTATTACK).down)
    {
        player->SetAttackType(true);
        player->RequestAttack();
        return;
    }
    // ‹­UŒ‚“ü—Í
    if (key.GetInfo(KEY::KEY_TYPE::HEAVYATTACK).down)
    {
        player->SetAttackType(false);
        player->RequestAttack();
        return;
    }

    // ˆÚ“®“ü—Í
    const bool isMoving = key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now ||
        key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now ||
        key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now ||
        key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now;

    if (isMoving)
    {
        player->ChangeState<MoveState>();
    }

}

void IdleState::Update(ActorBase* owner)
{
}

void IdleState::Exit(ActorBase* owner)
{
    stateFrame_ = 0;
}