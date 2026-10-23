#pragma once
#include "../SceneBase.h"
class SceneGameOver :
    public SceneBase
{
public:
	SceneGameOver(void);
	~SceneGameOver(void);

	void SubLoad(void)override;
	void SubInit(void)override;
	void SubUpdate(void)override;
	void SubDraw(void)override;
	void SubRelease(void)override;
	SCENE_ID GetSceneID(void)const override { return SCENE_ID::GAMEOVER; }


};

