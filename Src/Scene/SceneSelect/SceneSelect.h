#pragma once
#include "../SceneBase.h"
class SceneSelect :
    public SceneBase
{
public:
    SceneSelect(void);
    ~SceneSelect(void);

    void SubLoad(void) override;
    void SubInit(void) override;
    void SubUpdate(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;

    SCENE_ID GetSceneID(void)const override { return SCENE_ID::SELECT; }


};

