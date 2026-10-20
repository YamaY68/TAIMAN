#include "ActorBase.h"
#include"../Common/AnimationController.h"
#include"../Manager/TimeManager/TimeManager.h"
#include "../Manager/ActorManager/ActorManager.h"
#include"Collider/Collider.h"

ActorBase::ActorBase(void)
{
}

ActorBase::~ActorBase(void)
{
}


void ActorBase::Load(void)
{
	SubLoad();
	LoadAnimation();
}

//‰Šú‰»
void ActorBase::Init(void)
{
	SubInit();
	InitCollider();
}
//XV
void ActorBase::Update(void)
{
	slowTime_ = TimeManager::GetInstance().GetTimeScale();
	SubUpdate();
	if (bodyType_ == BodyType::DYNAMIC){movement_.Update(slowTime_);}
}
//•`‰æ
void ActorBase::Draw(void)
{
	SubDraw();
}
//‰ð•ú
void ActorBase::Release(void)
{
	SubRelease();
}

void ActorBase::AddCollider(Collider collider)
{
	collider.parentPos_ = &transform_.pos;
	colliders_.push_back(collider);
}



void ActorBase::PostUpdate(void)
{
	transform_.Update();
	if (animationController_)animationController_->Update(slowTime_);
}