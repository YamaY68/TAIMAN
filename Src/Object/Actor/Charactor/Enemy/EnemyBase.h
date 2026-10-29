#pragma once
#include "../../ActorBase.h"

class EnemyBase :
    public ActorBase
{
public:
    EnemyBase(void);
    virtual ~EnemyBase(void);

private:
    bool canLockOn_;
    VECTOR lockOnPos_;
    int hp_;
};

