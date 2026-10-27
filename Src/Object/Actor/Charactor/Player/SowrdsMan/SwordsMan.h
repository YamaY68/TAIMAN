#pragma once
#include"../PlayerBase.h"
#include"../../../Collider/HitContext.h"

class SwordsMan :
    public PlayerBase
{
public:
    SwordsMan(CameraManager*cameramanager);
    ~SwordsMan(void)override;

    void SubLoad(void)override;
    void SubInit(void)override;
    void SubDraw(void)override;

    void SubRelease(void)override;
private:
    void InitCollider(void)override;
    void LoadAnimation(void)override;
    void InitAnimation(void)override;
    void CreateAttack(void)override;
    void OnHit(const HitContext& context)override;
    void RequestAttack(void)override;

};