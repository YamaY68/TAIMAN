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
	virtual ~SceneBase(void);

	//読み込み
	void Load(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Release(void);

	/// <summary>
	/// シーンIDを取得
	/// 各シーンのヘッダーでオーバーライドしてシーンIDを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual SCENE_ID GetSceneID(void)const = 0;

protected:
	virtual void SubLoad(void) = 0;
	virtual void SubInit(void) = 0;
	virtual void SubUpdate(void) = 0;
	virtual void SubDraw(void) = 0;
	virtual void SubRelease(void) = 0;
	virtual void InitUI(void) {};
	virtual void InitSE(void) {};

	//リソース管理
	ResourceManager& resourceManager_;
	//シーン管理
	SceneManager& sceneManager_;


};

