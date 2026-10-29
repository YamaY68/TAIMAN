#pragma once
#include "GameModeBase.h"

class GameModeBoss : public GameModeBase
{
public:
	GameModeBoss(SceneGame* game);
	~GameModeBoss(void) override = default;

	void Enter(void) override;
	void Update(void) override;
	void Exit(void) override;
	bool IsCleared(void) const override;

	void CreateActor(void) override;
	void RegisterEvent(void)override;
private:
};