#include <DxLib.h>
#include"../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::DeleteInstance(void)
{
	if (instance_ != nullptr)
	{
		instance_->Destroy();
		delete instance_;
		instance_ = nullptr;
	}
}

void ResourceManager::Init(void)
{
	using RES = Resource;
	using RES_T = TYPE;

	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	Resource* res = nullptr;
	//ステージ
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "SkyDome/sky.mv1");
	resourcesMap_.emplace(SRC::SKYDOME, res);

	//プレイヤー
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/SowrdMan/SowrdMan.mv1");
	resourcesMap_.emplace(SRC::PLAYER_SWORDSMAN, res);

	//敵
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/TestEnemy/TestEnemy.mv1");
	resourcesMap_.emplace(SRC::ENEMY_TEST, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
}

const Resource& ResourceManager::Load(SRC src)
{
	const Resource& res = _Load(src);
	if (res.type_ == TYPE::IDLE)
	{
		return dummy_;
	}
	return res;
}


int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == TYPE::IDLE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);

	return duId;
}


ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
