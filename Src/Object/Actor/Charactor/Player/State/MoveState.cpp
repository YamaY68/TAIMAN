#include "MoveState.h"
#include <cmath>
#include "CommonStates.h"
#include "../PlayerBase.h"
#include "../../../../Common/AnimationController.h"
#include "../../../../../Manager/Input/KeyManager.h"
#include"../../../Camera/CameraManager.h"

void MoveState::Enter(ActorBase* owner)
{
    isLoop_ = true;
    moveSpeed_ = 0.5f;
    if (auto* anim = owner->GetAnimationController())
    {
        anim->Play((int)PlayerBase::ANIMATION_TYPE::WALK);
    }
}

void MoveState::HandleInput(ActorBase* owner)
{
    auto* player = static_cast<PlayerBase*>(owner);
    auto& key = KeyManager::GetInstance();

    // 各種アクションへの遷移
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

    // 移動キーが離されたらアイドルへ遷移
    if (!hasMoveInput)
    {
        player->ChangeState<IdleState>();
        return;
    }

    // 歩き / 走りの速度とモーション適用
    auto* anim = player->GetAnimationController();
    if (key.GetInfo(KEY::KEY_TYPE::SHIFT).now)
    {
        moveSpeed_ = 3.0f;
        if (anim) anim->Play((int)PlayerBase::ANIMATION_TYPE::RUN);
    }
    else
    {
        moveSpeed_ = 1.0f;
        if (anim) anim->Play((int)PlayerBase::ANIMATION_TYPE::WALK);
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
        //水平回転角をラジアンで取得
        float cameraYaw = player->GetCameraManager()->GetCameraYaw();
        //カメラ向きを基準に入力方向を回転
        MATRIX camRotY = MGetRotY(cameraYaw);
        moveVec_ = VNorm(VTransform(inputDir, camRotY));

        //アクターの向き・座標の更新
        auto& trans = owner->GetTransform();
        trans.quaRot = Quaternion::Euler(VGet(0.0f, atan2f(moveVec_.x, moveVec_.z), 0.0f));
        owner->GetMoveMent().SetInputMovePow(VScale(moveVec_, moveSpeed_));
        //trans.pos = VAdd(trans.pos, VScale(moveVec_, moveSpeed_));
    }
}

void MoveState::Exit(ActorBase* owner)
{
    moveVec_ = { 0.0f, 0.0f, 0.0f };
    stateFrame_ = 0;

}