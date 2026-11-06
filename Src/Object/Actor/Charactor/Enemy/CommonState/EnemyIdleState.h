#pragma once
#include "../../../Common/IState.h"
class ActorBase;
class EnemyIdleState :
    public IState<ActorBase>
{
    void Enter(ActorBase* owner) override;
    void Update(ActorBase* owner) override;
    void Exit(ActorBase* owner) override;
};

