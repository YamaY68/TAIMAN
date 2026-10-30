#pragma once
#include <DxLib.h>
#include "../../../Common/IState.h"

class ActorBase;

class MoveState : public IState<ActorBase>
{
public:
    //進入
    void Enter(ActorBase* owner) override;
    //入力受付
    void HandleInput(ActorBase* owner) override;
    //更新
    void Update(ActorBase* owner) override;
    //離脱
    void Exit(ActorBase* owner) override;
    //移動ベクトルの取得
    const VECTOR& GetMoveVec(void) const { return moveVec_; }

private:
    VECTOR moveVec_ = { 0.0f, 0.0f, 0.0f };
    float moveSpeed_ = 0.5f;
};