#pragma once
#include "../FactoryBase.h"
class ActorBase;
class CameraManager;
class TestFactory :
    public FactoryBase<ActorBase>
{
public:
    TestFactory(CameraManager*cameraManager);
    ~TestFactory(void);
    std::vector<std::unique_ptr<ActorBase>>Create(void)override;
};

