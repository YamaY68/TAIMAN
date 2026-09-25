#include "Application.h"

#include<DxLib.h>
#include<EffekseerForDXLib.h>

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

void Application::Init(void)
{
    //アプリケーションの初期化設定
    SetWindowText("TAIMAN");

    //ウィドウサイズ

    //DxLibの初期化

    //エフェクシアの初期化

    //入力制御初期化

    //FPS初期化

    //シーン管理初期化

}

void Application::Run(void)
{
    
}

void Application::Destroy(void)
{
}

bool Application::IsInitFail(void) const
{
    return false;
}

bool Application::IsDestroyFail(void) const
{
    return false;
}

void Application::RequestExit(void)
{
}

Application::Application(void)
{
}

void Application::InitEffekseer(void)
{
}
