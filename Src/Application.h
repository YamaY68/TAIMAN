#pragma once

#include<string>
#include<memory>
#include"Common/Vector2.h"
class FPS;
class Application
{
public:
	//スクリーンサイズ
	static constexpr int SCREEN_WIDTH = 1280;
	static constexpr int SCREEN_HEIGHT = 720;

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_DATA;
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_EFFECT;
	static const std::string PATH_SOUND;
	static const std::string PATH_FONT;
	static const std::string PATH_SHADER;
	//-------------------------------------------

	//シングルトン
	//インスタンスを明示的に生成
	static void CreateInstance(void);
	//インスタンスの取得
	static Application& GetInstance(void);
	//インスタンスの破棄
	static void DeleteInstance(void);

	//初期化
	void Init(void);

	//ゲームループの開始
	void Run(void);

	//リソースの破棄
	void Destroy(void);

	//初期化判定
	bool IsInitFail(void)const;

	//解放判定
	bool IsDestroyFail(void)const;

	//外部からの終了要求
	void RequestExit(void);

private:
	//静的インスタンス
	static Application* instance_;
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);
	// コピーコンストラクタも同様
	Application(const Application& instance) = default;
	// デストラクタも同様
	~Application(void) = default;
	//FPS
	std::unique_ptr<FPS> fps_;
	//初期化失敗判定
	bool isInitFail_;
	//解放失敗判定
	bool isDestroyFail_;
	//エフェクシアの初期化
	void InitEffekseer(void);
};

