#pragma once
#include<DxLib.h>
#include "../../../Common/IState.h"

class ActorBase;

class GuardState : public IState<ActorBase>
{
public:
    void Enter(ActorBase* owner) override;
    void HandleInput(ActorBase* owner) override;
    void Update(ActorBase* owner) override;
    void Exit(ActorBase* owner) override;

private:
    VECTOR moveVec_ = { 0.0f, 0.0f, 0.0f };
};