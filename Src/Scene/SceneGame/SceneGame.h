#pragma once
#include "../SceneBase.h"
#include<vector>
#include<memory>

class ActorManager;
class CollisionManager;
class FactoryManager;
class TimeManager;
class CameraManager;
class GameModeBase;

class DebugFreeVirtualCamera;
class PlayerFollowVirtualCamera;

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

	void ChangeMode(std::unique_ptr<GameModeBase>nextMode);

	ActorManager* GetActorManager(void) const { return actorManager_.get(); }
	CollisionManager* GetCollisionManager(void) const { return collisionManager_.get(); }
	FactoryManager* GetFactoryManager(void) const { return factoryManager_.get(); }
	TimeManager* GetTimeManager(void) const { return timeManager_.get(); }
	CameraManager* GetCameraManager(void) const { return cameraManager_.get(); }

private:
	std::unique_ptr<ActorManager>     actorManager_;
	std::unique_ptr<CollisionManager> collisionManager_;
	std::unique_ptr<FactoryManager>   factoryManager_;
	std::unique_ptr<TimeManager>      timeManager_;
	std::unique_ptr<CameraManager>    cameraManager_;

	// 現在動作中のゲームモード（無双 / ボス）
	std::unique_ptr<GameModeBase>     currentMode_;

	std::unique_ptr< DebugFreeVirtualCamera>debugCamera_;
	std::unique_ptr<PlayerFollowVirtualCamera>followCamera_;
};

