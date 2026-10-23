#pragma once
#include "../../ActorBase.h"
class SkyDome :
    public ActorBase
{
public:
    SkyDome(void);
    ~SkyDome() = default;

    void SubLoad(void) override;
    void SubInit(void) override;
    void SubUpdate(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;
};

