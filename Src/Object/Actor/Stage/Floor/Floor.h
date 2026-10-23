#pragma once
#include "../../ActorBase.h"
class Floor :
    public ActorBase
{
public :
    Floor(void);
    ~Floor(void)override;

    void SubLoad(void)override;
    void SubInit(void)override;

    void SubDraw(void)override;
    void SubRelease(void)override;
    void InitCollider(void)override;
};

