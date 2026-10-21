#pragma once
#include "../FactoryBase.h"
class ActorBase;
class TestFactory :
    public FactoryBase<ActorBase>
{
public:
    TestFactory(void);
    ~TestFactory(void);
    std::vector<std::unique_ptr<ActorBase>>Create(void)override;
    
};

