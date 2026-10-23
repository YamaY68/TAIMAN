#pragma once
#include "../../../Common/IState.h"
#include <DxLib.h>

class ActorBase;

class EvadeState : public IState<ActorBase>
{
public:
    void Enter(ActorBase* owner) override;
    void HandleInput(ActorBase* owner) override;
    void Update(ActorBase* owner) override;
    void Exit(ActorBase* owner) override;

    void SetEvadeDirection(const VECTOR& dir) { evadeDirection_ = dir; }

private:
    VECTOR evadeDirection_ = { 0.0f, 0.0f, 0.0f };
};