#include "SceneTIile.h"
#include"../../Manager/Input/KeyManager.h"
#include"../../Manager/Scene/SceneManager.h"
SceneTitle::SceneTitle(void)
{
}

SceneTitle::~SceneTitle(void)
{
}

void SceneTitle::SubLoad(void)
{
}

void SceneTitle::SubInit(void)
{
}

void SceneTitle::SubUpdate(void)
{
	if (KeyManager::GetInstanec().GetInfo(KEY_TYPE::SPACE).down)
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::SELECT);
		return;
	}
}

void SceneTitle::SubDraw(void)
{
	DrawString(0, 0, "title", 0xffffff);
}

void SceneTitle::SubRelease(void)
{
}
