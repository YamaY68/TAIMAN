#include "SceneManager.h"

#include"../Loading/Loading.h"
#include"../Resource/ResourceManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
}

SceneManager&SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::DeleteInstance(void)
{
	if(instance_ != nullptr)
	{
		instance_->Destroy();
		delete instance_;
		instance_ = nullptr;
	}
}

void SceneManager::SetSceneFactory(SCENE_ID sceneID, SceneFactory factory)
{
	sceneFactories_[sceneID] = factory;
}

// 初期化
void Init(void)
{
	//ロード画面生成
	Loading::CreateInstance();
	Loading::GetInstance()->Init();
	Loading::GetInstance()->Load();

	//リソースマネージャー
	ResourceManager::CreateInstance();
	ResourceManager::GetInstance().Init();
	//UIマネージャー

	//イベントマネージャー

	//3D用の初期化

}

// 更新
void Update(void)
{}

// 描画
void Draw(void)
{}

// リソースの破棄
void Destroy(void)
{
}

void SceneManager::ResetDeltaTime(void)
{
}

void SceneManager::Init3D(void)
{
}
