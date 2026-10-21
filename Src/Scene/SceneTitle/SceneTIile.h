#pragma once
#include "../SceneBase.h"
class SceneTitle :
    public SceneBase
{
public:
    SceneTitle(void);
    ~SceneTitle(void);

    void SubLoad(void)override;
    void SubInit(void)override;
    void SubUpdate(void)override;
    void SubDraw(void)override;
    void SubRelease(void)override;
    SCENE_ID GetSceneID(void)const override { return SCENE_ID::TITLE; }
private:

};

