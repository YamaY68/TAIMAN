#pragma once
#include "../../ActorBase.h"

class EnemyBase :
    public ActorBase
{
public:
    enum class ANIMAITON_TYPE
    {
        NONE=-1,
        IDLE,
        WALK,

    };

    EnemyBase(void);
    virtual ~EnemyBase(void);
    //
    void SubLoad(void)override;
    //‰Šú‰»
    void SubInit(void)override;
    //XV
    void SubUpdate(void)override;
    //•`‰æ
    void SubDraw(void)override;
    //‰ğ•ú
    void SubRelease(void)override;

protected:
    virtual void InitState(void) {}
    int hp_;
};

