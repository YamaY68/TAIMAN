#pragma once
#include "../EnemyBase.h"
class TestEnemy :
    public EnemyBase
{
public:
    TestEnemy(void);
    ~TestEnemy(void) override;

    void SubLoad(void) override;
    void SubInit(void) override;
    void SubUpdate(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;

    void InitCollider(void) override;
    void OnHit(const HitContext& context) override;

private:
    void InitState(void)override;
    void LoadAnimation(void)override;
    void InitAnimation(void)override;
    void CreateState(void)override;
};

