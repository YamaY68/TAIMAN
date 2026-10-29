#pragma once

class SceneGame;
class CameraManager;
class GameModeBase
{
public:
	GameModeBase(SceneGame* game) : game_(game) {}
	virtual ~GameModeBase(void) = default;

	virtual void Enter(void) = 0;
	virtual void Update(void) = 0;
	virtual void Exit(void) = 0;
	virtual bool IsCleared(void) const = 0;
	virtual void CreateActor(void) {}

	virtual void SetCameraManager(CameraManager* cameraManager) { cameraManager_ = cameraManager; }
protected:
	virtual void RegisterEvent(void) {};
	SceneGame* game_;
	CameraManager* cameraManager_ = nullptr;
};