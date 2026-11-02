#pragma once
#include "../../ActorBase.h"

class EnemyBase :
    public ActorBase
{
public:
    EnemyBase(void);
    virtual ~EnemyBase(void);

    //‰Šú‰»
    void SubInit(void)override;
    //XV
    void SubUpdate(void)override;
    //•`‰æ
    void SubDraw(void)override;
    //‰ğ•ú
    void SubRelease(void)override;

protected:

    int hp_;
};

