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
	slowTime_ = 1.0f;
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
	//slowTime_ = TimeManager::GetInstance().GetTimeScale();
	SubUpdate();
	stateMachine_.Update();
	if (bodyType_ == BodyType::DYNAMIC){
		movement_.Update(slowTime_);
		transform_.pos = VAdd(transform_.pos, movement_.GetMovePow());
	}
	transform_.Update();
	if (animationController_)animationController_->Update();
}
//•`‰æ
void ActorBase::Draw(void)
{
	SubDraw();
	MV1DrawModel(transform_.modelId);
}
//‰ð•ú
void ActorBase::Release(void)
{
	SubRelease();
	if (animationController_)animationController_->Release();

}

void ActorBase::AddCollider(Collider collider)
{
	colliders_.push_back(collider);
	colliders_.back().parentPos_ = &transform_.pos;
}

void ActorBase::OnCollision(const HitContext& context)
{
	if (!context.isTrigger && bodyType_ == BodyType::DYNAMIC)
	{
		movement_.OnCollision(context.normal);
	}
}



void ActorBase::PostUpdate(void)
{
	transform_.Update();
	if (animationController_)animationController_->Update(slowTime_);
}