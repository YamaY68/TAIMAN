#pragma once
#include"SceneID.h"

class ResourceManager;
class SceneManager;

class SceneBase
{
public:
	//コンストラクタ
	SceneBase(void);
	//デストラクタ
	virtual ~SceneBase(void) = default;

	//読み込み
	void Load(void);
	//初期化処理
	void Init(void);
	// 更新
	void Update(void);
	// 描画処理
	void Draw(void);
	// 解放処理
	void Release(void);

	/// <summary>
	///	シーンIDを取得する
	/// それぞれのシーンでオーバーライドして、シーンIDを返すようにする
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual SCENE_ID GetSceneID(void)const = 0;

	void SetDebugDraw(bool isDebug) { isDebugDraw_ = isDebug; }
protected:
	//読み込み処理
	virtual void SubLoad(void) = 0;
	//初期化処理
	virtual void SubInit(void) = 0;
	//更新処理
	virtual void SubUpdate(void) = 0;
	//描画処理
	virtual void SubDraw(void) = 0;
	//解放処理
	virtual void SubRelease(void) = 0;

	//UI
	virtual void InitUI(void) {};
	//SE
	virtual void InitSE(void) {};

	//デバッグ用
	virtual void DebugDraw(void) {};

	bool isDebugDraw_ = false;

	//リソース管理
	ResourceManager& resMng_;
	//シーン管理
	SceneManager& sceneMng_;
};

