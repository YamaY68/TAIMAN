#include "SceneBase.h"
#include"../Manager/Resource/ResourceManager.h"
#include"../Manager/Scene/SceneManager.h"
#include"../Manager/Input/KeyManager.h"
#include"../Application.h"

SceneBase::SceneBase(void)
	: resMng_(ResourceManager::GetInstance())
	, sceneMng_(SceneManager::GetInstance())
{
}


void SceneBase::Load(void)
{
	SubLoad();
}

void SceneBase::Init(void)
{
	SubInit();
	InitUI();
	InitSE();
}

void SceneBase::Update(void)
{
	if (SceneManager::GetInstance().GetSceneID() != SCENE_ID::PAUSE){
		if (KeyManager::GetInstanec().GetInfo(KEY_TYPE::PAUSE).down){
			SceneManager::GetInstance().PushScene(SCENE_ID::PAUSE);
			return;
		}
	}
	SubUpdate();
}

void SceneBase::Draw(void)
{
	SubDraw();
	if(isDebugDraw_){
		DebugDraw();
	}
}

void SceneBase::Release(void)
{
	SubRelease();
}