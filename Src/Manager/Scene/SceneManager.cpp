#include "SceneManager.h"
#include<EffekseerForDXLib.h>

#include"../Loading/Loading.h"
#include"../Resource/ResourceManager.h"
#include"../Input/KeyManager.h"

#include"../../Scene/SceneBase.h"

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

SceneManager::SceneManager(void)
{
}


void SceneManager::SetSceneFactory(SCENE_ID sceneID, SceneFactory factory)
{
	sceneFactories_[sceneID] = factory;
}

void SceneManager::Init(void)
{
	//ロード画面
	Loading::CreateInstance();
	Loading::GetInstance()->Load();
	Loading::GetInstance()->Init();

	//リソースマネージャー
	ResourceManager::CreateInstance();
	ResourceManager::GetInstance().Init();

	//UIマネージャー

	//イベントマネージャー

	//３Dの設定
	Init3D();

	//デルタタイム
	preTime_ = std::chrono::system_clock::now();

	//シーンの初期化
	ChangeScene(SCENE_ID::TITLE);

	mainScreen_ = MakeScreen(Application::SCREEN_WIDTH, Application::SCREEN_HEIGHT, true);
}

// ChangeScene は即座に実体化せず「予約」だけにする
void SceneManager::ChangeScene(SCENE_ID scene)
{
	nextSceneID_ = scene;
	isChangeRequested_ = true;
}

// （shared_ptr を直接受ける版も予約にするか、SCENE_ID 版に統一する）
void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	if (!scene) return;
	// 直接 shared_ptr で受ける場合も予約パターンにするか、基本は SCENE_ID 経由に寄せる
	nextSceneID_ = scene->GetSceneID();
	isChangeRequested_ = true;
}

void SceneManager::Update(void)
{
	if (isChangeRequested_)
	{
		auto it = sceneFactories_.find(nextSceneID_);
		if (it != sceneFactories_.end())
		{
			// 旧シーンの解放
			if (!scenes_.empty())
			{
				scenes_.back()->Release();
				scenes_.pop_back();
			}

			// 新シーンの実体化と初期化
			auto newScene = it->second();
			scenes_.push_back(newScene);
			scenes_.back()->Load();
			scenes_.back()->Init();

			sceneID_ = nextSceneID_;
		}
		isChangeRequested_ = false;
		ResetDeltaTime(); // ロード時間でデルタタイムが跳ねるのを防止
	}

	// シーンがなければ終了
	if (scenes_.empty()) { return; }

	// デルタタイム計算
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	if (Loading::GetInstance()->IsLoading())
	{
		Loading::GetInstance()->Update();
		if (Loading::GetInstance()->IsLoading() == false)
		{
			scenes_.back()->Init();
		}
	}
	else
	{
		scenes_.back()->Update();
	}
}

void SceneManager::Draw(void)
{
	//描画先の指定
	SetDrawScreen(mainScreen_);

	//画面を初期化
	ClearDrawScreen();

	//エフェクシアの更新
	UpdateEffekseer3D();

	if (Loading::GetInstance()->IsLoading()) {
		Loading::GetInstance()->Draw();
	}
	else{
		//camera->SetBeforeDraw();
		for(auto&scene : scenes_){
			scene->Draw();
		}
	}
	//エフェクシアの描画
	DrawEffekseer3D();
	//UI
	//UIManager::GetInstance().Draw();
	
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawRotaGraph(Application::SCREEN_WIDTH / 2, Application::SCREEN_HEIGHT / 2, 1, 0, mainScreen_, true);
}

void SceneManager::Destroy(void)
{
	//シーンの開放
	for (auto& scene : scenes_) { scene->Release(); }
	scenes_.clear();

	DeleteGraph(mainScreen_);

	//ロード画面の削除
	Loading::GetInstance()->Release();
	Loading::GetInstance()->DeleteInstance();
	//EventManager::DeleteInstance();
	//UIManager::DeleteInstance();

	//インスタンスメモリの解放
	delete instance_;
	instance_ = nullptr;
}

void SceneManager::PushScene(std::shared_ptr<SceneBase>scene)
{
	//新しく積むのでもともと入っている奴はまだ削除されない
	scenes_.push_back(scene);
	scenes_.back()->Load();
	scenes_.back()->Init();
	//UIManager::GetInstance().Init();
	sceneID_ = scenes_.back()->GetSceneID();
}

void SceneManager::PushScene(SCENE_ID scene)
{
	auto it = sceneFactories_.find(scene);
	//登録されているシーンのレシピがあれば、シーンを切り替える
	if (it != sceneFactories_.end()){
		sceneID_ = scene;
		//登録されたラムダ式を実行して実体を生成
		PushScene(it->second());
	}
}

void SceneManager::PopScene(void)
{
	//シーンが一つしかない場合は削除しない
	if (scenes_.size() <= 1) { return; }
	scenes_.back()->Release();
	scenes_.pop_back();
	sceneID_ = scenes_.back()->GetSceneID();
}

//void SceneManager::ResetScene(std::shared_ptr<SceneBase> scene)
//{
//	// 全て解放
//	for (auto& scene : scenes_) { scene->Release(); }
//	scenes_.clear();
//	scenes_.push_back(scene);
//
//	// 新しく積む
//	ChangeScene(scene);
//}
//
//void SceneManager::JumpScene(std::shared_ptr<SceneBase>scene)
//{
//}
//
//void SceneManager::JumpScene(SCENE_ID scene)
//{
//}

float SceneManager::GetTotalGameTime(void)
{
	return totalGameTime_;
}

void SceneManager::SetTotalGameTime(float time)
{
	totalGameTime_ = time;
}

void SceneManager::ForwardGameTime(void)
{
	totalGameTime_ += GetDeltaTime();
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(0, 139, 139);
	// Zバッファを有効にする
	SetUseZBuffer3D(true);
	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);
	// バックカリングを有効にする
	SetUseBackCulling(true);
	// ライトを有効にする
	SetUseLighting(true);
	// ディレクショナルライト方向の設定(正規化されていなくても良い)
	// 正面から斜め下に向かったライト
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

	//// フォグ設定
	//SetFogEnable(true);
	//// フォグの色
	//SetFogColor(100, 100, 100);
	//// フォグを発生させる奥行きの最小、最大距離
	//SetFogStartEnd(500 , 5000 );

}

void SceneManager::ApplyChangeScene()
{
}
