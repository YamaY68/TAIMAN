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
        anim->Play((int)PlayerBase::ANIMATION_TYPE::GUARD);
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
    auto* anim = owner->GetAnimationController();

    VECTOR inputDir = { 0.0f, 0.0f, 0.0f };
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now) inputDir.z += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now)  inputDir.z -= 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now) inputDir.x += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now)  inputDir.x -= 1.0f;

    // 移動キー入力なしの場合は後退ベクトルを設定してブレンド解除
    if (VSquareSize(inputDir) <= 0.0001f)
    {
        moveVec_ = player->GetTransform().GetBack();
        if (anim) anim->RemoveBlendAnim((int)PlayerBase::ANIMATION_TYPE::WALK);
        return;
    }
    // 移動キー入力あり：カメラ方向を考慮して移動ベクトルを算出
    float cameraYaw = player->GetCameraManager()->GetCameraYaw();
    MATRIX camRotY = MGetRotY(cameraYaw);
    moveVec_ = VNorm(VTransform(inputDir, camRotY));

    owner->GetMoveMent().SetInputMovePow(VScale(moveVec_, 0.25f));
    if (anim) anim->SetBlendAnim((int)PlayerBase::ANIMATION_TYPE::WALK, 0.3f, true);
}

void GuardState::Exit(ActorBase* owner)
{
    if (auto* anim = owner->GetAnimationController())
    {
        anim->RemoveBlendAnim((int)PlayerBase::ANIMATION_TYPE::WALK);
    }
        stateFrame_ = 0;
        moveVec_ = ZERO;
}