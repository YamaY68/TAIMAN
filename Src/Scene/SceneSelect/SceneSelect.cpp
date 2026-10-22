#include "SceneSelect.h"
#include"../../Manager/Input/KeyManager.h"
#include"../../Manager/Scene/SceneManager.h"


SceneSelect::SceneSelect(void)
{
}

SceneSelect::~SceneSelect(void)
{
}

void SceneSelect::SubLoad(void)
{
}

void SceneSelect::SubInit(void)
{
}

void SceneSelect::SubUpdate(void)
{
	if (KEY::GetInstance().GetInfo(KEY::KEY_TYPE::SPACE).down)
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME);
	}
}

void SceneSelect::SubDraw(void)
{
	DrawFormatString(0, 0, 0xffffff, "Select");
}

void SceneSelect::SubRelease(void)
{
}
