#include "GuardState.h"
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"
#include "../../../../../Manager/Input/KeyManager.h"

void GuardState::Enter(ActorBase* owner)
{
    isLoop_ = true;
    if (auto* anim = owner->GetAnimationController())
    {
        anim->Play((int)PlayerBase::ACTION_TYPE::GUARD);
    }
}

void GuardState::HandleInput(ActorBase* owner)
{
    auto* player = static_cast<PlayerBase*>(owner);
    auto& key = KeyManager::GetInstance();

    // ガードボタンが離されたら終了
    if (!key.GetInfo(KEY::KEY_TYPE::GUARD).now)
    {
        player->ChangeState<IdleState>();
        return;
    }

    // ガード中の派生入力
    if (key.GetInfo(KEY::KEY_TYPE::PARRY).down)
    {
        player->ChangeState<ParryState>();
        return;
    }
    if (key.GetInfo(KEY::KEY_TYPE::EVADE).down)
    {
        player->ChangeState<EvadeState>();
        return;
    }
}

void GuardState::Update(ActorBase* owner)
{
    auto& key = KeyManager::GetInstance();

    VECTOR moveVec = { 0.0f, 0.0f, 0.0f };
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now) moveVec.z += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now)  moveVec.z -= 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now) moveVec.x += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now)  moveVec.x -= 1.0f;

    auto* anim = owner->GetAnimationController();
    if (VSquareSize(moveVec) > 0.0001f)
    {
        if (anim) anim->SetBlendAnim((int)PlayerBase::ACTION_TYPE::WALK, 0.3f, true);
        moveVec = VNorm(moveVec);
        owner->GetMoveMent().SetInputMovePow(VScale(moveVec, 0.25));
    }
    else
    {
        if (anim) anim->RemoveBlendAnim((int)PlayerBase::ACTION_TYPE::WALK);
    }
}

void GuardState::Exit(ActorBase* owner)
{
    if (auto* anim = owner->GetAnimationController())
    {
        anim->RemoveBlendAnim((int)PlayerBase::ACTION_TYPE::WALK);
    }
}