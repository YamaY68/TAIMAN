#include "SceneBase.h"

#include"../Manager/Resource/ResourceManager.h"
#include"../Manager/Scene/SceneManager.h"
#include"../Manager/Input/KeyManager.h"
#include"../Application.h"

SceneBase::SceneBase(void)
	:resourceManager_(ResourceManager::GetInstance()),
	sceneManager_(SceneManager::GetInstance())
{
}

SceneBase::~SceneBase(void)
{
}

//“Ç‚İ‚İ
void Load(void)
{
	
}

//‰Šú‰»
void Init(void)
{

}

//XV
void Update(void)
{

}

//•`‰æ
void Draw(void)
{

}

//‰ğ•ú
void Release(void)
{

}
