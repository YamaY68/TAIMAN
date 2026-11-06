#include "GuardState.h"
#include <cmath>
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"
#include "../../../../../Manager/Input/KeyManager.h"
#include "../../../Camera/CameraManager.h"

void GuardState::Enter(ActorBase* owner)
{
    isLoop_ = true;
    if (auto* anim = owner->GetAnimationController())
    {
        anim->Play((int)PlayerBase::ANIMATION_TYPE::GUARD);
    }

    // ガード開始時にもロックオン中なら即座に敵を向く
    auto* player = static_cast<PlayerBase*>(owner);
    if (player->IsLockon())
    {
        player->RotateTowardsIntendInput();
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
    auto& trans = owner->GetTransform();
    auto* camManager = player->GetCameraManager();

    // 1. ロックオン中の向き更新（静止中・移動中問わず常に敵を向く）
    if (player->IsLockon() && camManager)
    {
        const auto& camState = camManager->GetCurrentState();
        VECTOR toTarget = VSub(camState.target, trans.pos);
        toTarget.y = 0.0f;

        if (VSquareSize(toTarget) > 0.0001f)
        {
            trans.quaRot = Quaternion::Euler(VGet(0.0f, atan2f(toTarget.x, toTarget.z), 0.0f));
        }
    }

    // 2. 移動入力の取得
    VECTOR inputDir = { 0.0f, 0.0f, 0.0f };
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now) inputDir.z += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now)  inputDir.z -= 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now) inputDir.x += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now)  inputDir.x -= 1.0f;

    // 移動入力がない場合
    if (VSquareSize(inputDir) <= 0.0001f)
    {
        moveVec_ = trans.GetBack(); // 回避用デフォルト方向
        if (anim) anim->RemoveBlendAnim((int)PlayerBase::ANIMATION_TYPE::WALK);
        return;
    }

    // 移動入力がある場合：カメラ方向を考慮して移動ベクトルを算出
    if (camManager)
    {
        float cameraYaw = camManager->GetCameraYaw();
        MATRIX camRotY = MGetRotY(cameraYaw);
        moveVec_ = VNorm(VTransform(inputDir, camRotY));
    }

    // 非ロックオン時のみ、入力・進行方向を向く
    if (!player->IsLockon())
    {
        trans.quaRot = Quaternion::Euler(VGet(0.0f, atan2f(moveVec_.x, moveVec_.z), 0.0f));
    }

    // ガード歩き速度（0.25f）で移動推進力を設定
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
    moveVec_ = { 0.0f, 0.0f, 0.0f };
}