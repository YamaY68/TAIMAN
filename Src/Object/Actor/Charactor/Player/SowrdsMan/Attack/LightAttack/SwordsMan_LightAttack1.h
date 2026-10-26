#pragma once
#include "../../../../../Common/IState.h"
class ActorBase;

class SwordsMan_LightAttack1 :
    public IState<ActorBase>
{
public:
    void Enter(ActorBase* owner) override;
    void HandleInput(ActorBase* owner) override;
    void Update(ActorBase* owner) override;
    void Exit(ActorBase* owner) override;
};

