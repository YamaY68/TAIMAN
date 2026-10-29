#include "GuardState.h"
#include<cmath>
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"
#include "../../../../../Manager/Input/KeyManager.h"
#include"../../../Camera/CameraManager.h"

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
        auto* evade = player->GetState<EvadeState>();
        if (evade)
        {
            evade->SetEvadeDirection(moveVec_);
        }
        player->ChangeState<EvadeState>();
        return;
    }
}

void GuardState::Update(ActorBase* owner)
{
    auto& key = KeyManager::GetInstance();
    auto* player = static_cast<PlayerBase*>(owner);

    VECTOR inputDir = { 0.0f, 0.0f, 0.0f };
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now) inputDir.z += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now)  inputDir.z -= 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now) inputDir.x += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now)  inputDir.x -= 1.0f;

    auto* anim = owner->GetAnimationController();
    if (VSquareSize(inputDir) > 0.0001f)
    {
        float cameraYaw = player->GetCameraManager()->GetCameraYaw();
        MATRIX camRotY = MGetRotY(cameraYaw);
        moveVec_ = VNorm(VTransform(inputDir, camRotY));
        owner->GetMoveMent().SetInputMovePow(VScale(moveVec_, 0.25));
        if (anim) anim->SetBlendAnim((int)PlayerBase::ACTION_TYPE::WALK, 0.3f, true);
    }
    else
    {
        moveVec_ = player->GetTransform().GetBack();
        if (anim) anim->RemoveBlendAnim((int)PlayerBase::ACTION_TYPE::WALK);
    }
}

void GuardState::Exit(ActorBase* owner)
{
    if (auto* anim = owner->GetAnimationController())
    {
        anim->RemoveBlendAnim((int)PlayerBase::ACTION_TYPE::WALK);
    }
        stateFrame_ = 0;
        moveVec_ = ZERO;
}