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
        anim->Play((int)PlayerBase::ANIM_TYPE::IDLE);
    }
}

void IdleState::HandleInput(ActorBase* owner)
{
    auto* player = static_cast<PlayerBase*>(owner);
    auto& key = KeyManager::GetInstance();

    // ˆÚ“®“ü—Í
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now ||
        key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now ||
        key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now ||
        key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now)
    {
        player->ChangeState<MoveState>();
        return;
    }

    // ƒK[ƒh“ü—Í
    if (key.GetInfo(KEY::KEY_TYPE::GUARD).now)
    {
        player->ChangeState<GuardState>();
        return;
    }

    // ƒ‰ƒCƒgUŒ‚
    if (key.GetInfo(KEY::KEY_TYPE::LIGHTATTACK).down)
    {
        player->SetAttackType(true);
        player->RequestAttack();
        return;
    }

    // ƒwƒ”ƒBUŒ‚
    if (key.GetInfo(KEY::KEY_TYPE::HEAVYATTACK).down)
    {
        player->SetAttackType(false);
        player->RequestAttack();
        return;
    }
}

void IdleState::Update(ActorBase* owner)
{
}

void IdleState::Exit(ActorBase* owner)
{
}