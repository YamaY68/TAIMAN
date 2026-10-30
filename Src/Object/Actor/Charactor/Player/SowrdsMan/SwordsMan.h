#pragma once
#include"../PlayerBase.h"
#include"../../../Collider/HitContext.h"

class SwordsMan :
    public PlayerBase
{
public:
    //コンストラクタ
    SwordsMan(CameraManager*cameramanager);
    //デストラクタ
    ~SwordsMan(void)override;
    //読み込み
    void SubLoad(void)override;
    //初期化
    void SubInit(void)override;
    //描画
    void SubDraw(void)override;
    //解放
    void SubRelease(void)override;
private:
    void InitCollider(void)override;
    void LoadAnimation(void)override;
    void InitAnimation(void)override;
    void CreateAttack(void)override;
    void OnHit(const HitContext& context)override;
    void RequestAttack(void)override;

};