#pragma once
#include <map>
#include <string>
#include "Resource.h"
#include"ResourceName.h"

class ResourceManager
{
public:
	static void CreateInstance(void);
	static ResourceManager& GetInstance(void);
	static void DeleteInstance(void);
private:
	// 静的インスタンス
	static ResourceManager* instance_;
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

public:

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:
	// リソース管理の対象
	std::map<SRC, Resource*> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;
	// 内部ロード
	Resource& _Load(SRC src);
};

