#pragma once
#include <DxLib.h>
#include "../../../Common/IState.h"

class ActorBase;

class MoveState : public IState<ActorBase>
{
public:
    void Enter(ActorBase* owner) override;
    void HandleInput(ActorBase* owner) override;
    void Update(ActorBase* owner) override;
    void Exit(ActorBase* owner) override;

    const VECTOR& GetMoveVec(void) const { return moveVec_; }

private:
    VECTOR moveVec_ = { 0.0f, 0.0f, 0.0f };
    float moveSpeed_ = 0.5f;
};