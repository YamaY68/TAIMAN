#include "Application.h"

#include<DxLib.h>
#include<EffekseerForDXLib.h>

#include"Manager/Input/KeyManager.h"
#include"Manager/Scene/SceneManager.h"
#include"Manager/FPS/FPS.h"

#include"Scene/SceneTitle/SceneTIile.h"
#include"Scene/SceneSelect/SceneSelect.h"
#include"Scene/SceneGame/SceneGame.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_DATA = "Data/";
const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
const std::string Application::PATH_MODEL = PATH_DATA + "Model/";
const std::string Application::PATH_EFFECT = PATH_DATA + "Effect/";
const std::string Application::PATH_SOUND = PATH_DATA + "Sound/";
const std::string Application::PATH_FONT = PATH_DATA + "Font/";
const std::string Application::PATH_SHADER = PATH_DATA + "Shader/";

void Application::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new Application();
    }
    instance_->Init();
}

Application& Application::GetInstance(void)
{
    return *instance_;
}

void Application::DeleteInstance(void)
{
    if (instance_ != nullptr)
    {
        instance_->Destroy();
        delete instance_;
        instance_ = nullptr;
    }
}

Application::Application(void)
{
}

void Application::Init(void)
{
    //アプリケーションの初期化設定
    SetWindowText("TAIMAN");

    //ウィドウサイズ
    ChangeWindowMode(true);
    SetOutApplicationLogValidFlag(TRUE);
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

    //DxLibの初期化
    SetUseDirect3DVersion(DX_DIRECT3D_11);

    SetAlwaysRunFlag(TRUE);                  // バックグラウンドでも停止させない
    SetWaitVSyncFlag(FALSE);                 // 垂直同期の同期待ちを一旦オフにする
    // SetEnableWASAPIFlag(FALSE);           // ※もし下で止まる場合はこれも試す

    isInitFail_ = false;
    if (DxLib_Init() == -1)
    {
        isInitFail_ = true;
        return;
    }
    //エフェクシアの初期化
    InitEffekseer();

    //入力制御初期化
    KeyManager::CreateIns();

    //FPS初期化
    fps_ = std::make_unique<FPS>();
    fps_->Init();

    //シーン管理初期化
    SceneManager::CreateInstance();
    SceneManager::GetInstance().SetSceneFactory(SCENE_ID::TITLE, []() {return std::make_shared<SceneTitle>(); });
    SceneManager::GetInstance().SetSceneFactory(SCENE_ID::SELECT, []() {return std::make_shared<SceneSelect>(); });
    SceneManager::GetInstance().SetSceneFactory(SCENE_ID::GAME, []() {return std::make_shared<SceneGame>(); });
    SceneManager::GetInstance().Init();

}

void Application::Run(void)
{
    while (ProcessMessage() == 0 && !isRequestedExit_)
    {
        if (!fps_->UpdateFrameRate())continue;
        KEY::GetInstance().Update();
        SceneManager::GetInstance().Update();
        fps_->DrawFrameRate();
        SceneManager::GetInstance().Draw();

        ScreenFlip();
    }
}

void Application::Destroy(void)
{
    SceneManager::GetInstance().Destroy();

    KeyManager::GetInstance().DeleteIns();

    Effkseer_End();

    if (DxLib_End() == -1)
    {
        isDestroyFail_;
    }
}

bool Application::IsInitFail(void) const
{
    return isInitFail_;
}

bool Application::IsDestroyFail(void) const
{
    return isDestroyFail_;
}

void Application::RequestExit(void)
{
    isRequestedExit_ = true;
}

void Application::InitEffekseer(void)
{
    if (Effekseer_Init(8000) == -1)
    {
        DxLib_End();
    }
}
