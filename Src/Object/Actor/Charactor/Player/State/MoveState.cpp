// MoveState.cpp
#include "MoveState.h"
#include <cmath>
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"
#include "../../../../../Manager/Input/KeyManager.h"
#include "../../../Camera/CameraManager.h"
#include "../../../Camera/LockOnVirtualCamera.h"

void MoveState::Enter(ActorBase* owner)
{
    isLoop_ = true;
    moveSpeed_ = 1.0f;
    if (auto* anim = owner->GetAnimationController())
    {
        anim->Play((int)PlayerBase::ANIMATION_TYPE::WALK);
    }
}

void MoveState::HandleInput(ActorBase* owner)
{
    auto* player = static_cast<PlayerBase*>(owner);
    auto& key = KeyManager::GetInstance();

    // 回避・ガード・攻撃への派生
    if (key.GetInfo(KEY::KEY_TYPE::EVADE).down)
    {
        auto* evade = player->GetState<EvadeState>();
        if (evade) evade->SetEvadeDirection(moveVec_);
        player->ChangeState<EvadeState>();
        return;
    }
    if (key.GetInfo(KEY::KEY_TYPE::GUARD).down)
    {
        player->ChangeState<GuardState>();
        return;
    }
    if (key.GetInfo(KEY::KEY_TYPE::LIGHTATTACK).down)
    {
        player->SetAttackType(true);
        player->RequestAttack();
        return;
    }
    if (key.GetInfo(KEY::KEY_TYPE::HEAVYATTACK).down)
    {
        player->SetAttackType(false);
        player->RequestAttack();
        return;
    }

    // 移動キー入力の有無を確認
    const bool hasMoveInput = key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now ||
        key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now ||
        key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now ||
        key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now;

    if (!hasMoveInput)
    {
        player->ChangeState<IdleState>();
        return;
    }

    // 速度の設定
    if (key.GetInfo(KEY::KEY_TYPE::SHIFT).now)
    {
        moveSpeed_ = 3.0f;
    }
    else
    {
        moveSpeed_ = 1.0f;
    }
}

void MoveState::Update(ActorBase* owner)
{
    auto& key = KeyManager::GetInstance();
    VECTOR inputDir = { 0.0f, 0.0f, 0.0f };
    auto* player = static_cast<PlayerBase*>(owner);

    if (key.GetInfo(KEY::KEY_TYPE::MOVE_FRONT).now) inputDir.z += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_BACK).now)  inputDir.z -= 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_RIGHT).now) inputDir.x += 1.0f;
    if (key.GetInfo(KEY::KEY_TYPE::MOVE_LEFT).now)  inputDir.x -= 1.0f;

    if (VSquareSize(inputDir) > 0.0001f)
    {
        auto* camManager = player->GetCameraManager();
        float cameraYaw = camManager->GetCameraYaw();
        MATRIX camRotY = MGetRotY(cameraYaw);
        moveVec_ = VNorm(VTransform(inputDir, camRotY));

        auto& trans = owner->GetTransform();
        const bool isRunning = key.GetInfo(KEY::KEY_TYPE::SHIFT).now;
        auto* anim = player->GetAnimationController();

        if (isRunning)
        {
            // 【走り】移動方向を向き、走りモーション
            trans.quaRot = Quaternion::Euler(VGet(0.0f, atan2f(moveVec_.x, moveVec_.z), 0.0f));
            if (anim) anim->Play((int)PlayerBase::ANIMATION_TYPE::RUN);
        }
        else if (player->IsLockon())
        {
            // 【ロックオン歩き】注視点（ターゲット）を向く
            const auto& camState = camManager->GetCurrentState();
            VECTOR toTarget = VSub(camState.target, trans.pos);
            toTarget.y = 0.0f;

            if (VSquareSize(toTarget) > 0.0001f)
            {
                trans.quaRot = Quaternion::Euler(VGet(0.0f, atan2f(toTarget.x, toTarget.z), 0.0f));
            }

            // ★ 左右入力に応じたモーションの再生
            if (anim)
            {
                if (inputDir.x > 0.1f)
                {
                    anim->Play((int)PlayerBase::ANIMATION_TYPE::RIGHT_WALK);
                }
                else if (inputDir.x < -0.1f)
                {
                    anim->Play((int)PlayerBase::ANIMATION_TYPE::LEFT_WALK);
                }
                else
                {
                    // 前後移動（WまたはS）
                    anim->Play((int)PlayerBase::ANIMATION_TYPE::WALK);
                }
            }
        }
        else
        {
            // 【通常歩き】移動方向を向き、通常歩きモーション
            trans.quaRot = Quaternion::Euler(VGet(0.0f, atan2f(moveVec_.x, moveVec_.z), 0.0f));
            if (anim) anim->Play((int)PlayerBase::ANIMATION_TYPE::WALK);
        }

        owner->GetMoveMent().SetInputMovePow(VScale(moveVec_, moveSpeed_));
    }
}

void MoveState::Exit(ActorBase* owner)
{
    moveVec_ = { 0.0f, 0.0f, 0.0f };
    stateFrame_ = 0;
}