#pragma once
#include "../SceneBase.h"
#include<vector>
#include<memory>

class ActorManager;
class CollisionManager;
class FactoryManager;
class TimeManager;

class SceneGame :
    public SceneBase
{
public:
    SceneGame(void);
    ~SceneGame(void);

	void SubLoad(void)override;
	void SubInit(void)override;
	void SubUpdate(void)override;
	void SubDraw(void)override;
	void SubRelease(void)override;
	SCENE_ID GetSceneID(void)const override { return SCENE_ID::GAME; }

private:
	std::unique_ptr<ActorManager>actorManager_;
	std::unique_ptr<CollisionManager>collisionManager_;
	std::unique_ptr<FactoryManager>factoryManager_;
	std::unique_ptr<TimeManager>timeManager_;

};

