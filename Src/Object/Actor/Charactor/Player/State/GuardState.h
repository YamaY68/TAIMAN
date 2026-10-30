#pragma once
#include<DxLib.h>
#include "../../../Common/IState.h"

class ActorBase;

class GuardState : public IState<ActorBase>
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

private:
    VECTOR moveVec_ = { 0.0f, 0.0f, 0.0f };
};