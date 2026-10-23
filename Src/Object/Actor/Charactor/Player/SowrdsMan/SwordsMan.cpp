#include "SwordsMan.h"
#include<memory>
#include"../../../../Common/AnimationController.h"
#include"../../../../../Utility/AsoUtility.h"
#include"../State/CommonStates.h"

const std::string ANIM_PATH = "Data/Model/Player/SowrdMan/";

SwordsMan::SwordsMan(void)
{
}

SwordsMan::~SwordsMan(void)
{
}

void SwordsMan::SubLoad(void)
{
	transform_.modelId = MV1LoadModel("Data/Model/Player/SowrdMan/Sowrdman.mv1");
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	transform_.quaRotLocal = Quaternion::Euler(VGet(0, AsoUtility::Deg2RadF(180), 0));
	transform_.scl = VGet(0.5f, 0.5f, 0.5f);
	CreateAttack();
	LoadAnimation();
}

void SwordsMan::SubInit(void)
{
	InitAnimation();
	bodyType_ = BodyType::DYNAMIC;
	ChangeState<IdleState>();
}

void SwordsMan::SubDraw(void)
{
	//DrawSphere3D(VGet(0, 0, 0), 16, 15, 0xff0000, 0xff0000, true);
}

void SwordsMan::SubRelease(void)
{
}

void SwordsMan::InitCollider(void)
{
	Collider collider;
	collider.shape = CollisionShape::CAPSULE;
	collider.layer = ColliderLayer::ACTOR;
	collider.mask = ColliderFilter::MakeMask({ ColliderLayer::ACTOR,ColliderLayer::STAGE });
	collider.isActive = true;
	collider.isTrigger = false;
	collider.capsule.radius = 10.0f;
	collider.capsule.localTop = VGet(0.0f, 10.0f, 0.0f);
	collider.capsule.localDown = VGet(0.0f, -10.0f, 0.0f);
	AddCollider(collider);
}

void SwordsMan::LoadAnimation(void)
{
	animationController_->Add((int)ANIM_TYPE::IDLE, 30, ANIM_PATH + "Idle.mv1");
	animationController_->Add((int)ANIM_TYPE::WALK, 15, ANIM_PATH + "Walk.mv1");
	animationController_->Add((int)ANIM_TYPE::RUN, 38, ANIM_PATH + "Run.mv1");
	animationController_->Add((int)ANIM_TYPE::GUARD, 30, ANIM_PATH + "Guard.mv1");
	animationController_->Add((int)ANIM_TYPE::EVADE, 90, ANIM_PATH + "Evade.mv1");
	animationController_->Add((int)ANIM_TYPE::BACK_EVADE, 90, ANIM_PATH + "BackEvade.mv1");
	animationController_->Add((int)ANIM_TYPE::PARRY, 90, ANIM_PATH + "Parry.mv1");
	animationController_->Add((int)ANIM_TYPE::LEFT_STRAFE_WALK, 30, ANIM_PATH + "LeftStrafeWalk.mv1");
	animationController_->Add((int)ANIM_TYPE::LEFT_STRAFE_RUN, 30, ANIM_PATH + "LeftStrafeRun.mv1");
	animationController_->Add((int)ANIM_TYPE::RIGHT_STRAFE_WALK, 30, ANIM_PATH + "RightStrafeWalk.mv1");
	animationController_->Add((int)ANIM_TYPE::RIGHT_STRAFE_RUN, 30, ANIM_PATH + "RightStrafeRun.mv1");
	animationController_->Add((int)ANIM_TYPE::LIGHT_ATTACK_1, 110, ANIM_PATH + "LightAttack/LightAttack1.mv1");
	animationController_->Add((int)ANIM_TYPE::LIGHT_ATTACK_2, 110, ANIM_PATH + "LightAttack/LightAttack2.mv1");
	animationController_->Add((int)ANIM_TYPE::LIGHT_ATTACK_3, 110, ANIM_PATH + "LightAttack/LightAttack3.mv1");
	animationController_->Add((int)ANIM_TYPE::LIGHT_ATTACK_4, 110, ANIM_PATH + "LightAttack/LightAttack4.mv1");
	animationController_->Add((int)ANIM_TYPE::HEAVY_ATTACK_1, 90, ANIM_PATH + "HeavyAttack/HeavyAttack1.mv1");
	animationController_->Add((int)ANIM_TYPE::HEAVY_ATTACK_2, 90, ANIM_PATH + "HeavyAttack/HeavyAttack2.mv1");
	animationController_->Add((int)ANIM_TYPE::HEAVY_ATTACK_3, 90, ANIM_PATH + "HeavyAttack/HeavyAttack3.mv1");
	animationController_->Add((int)ANIM_TYPE::FINISH, 60, ANIM_PATH + "FinishAttack/Finish.mv1");
}
void SwordsMan::InitAnimation(void)
{
}

void SwordsMan::CreateAttack(void)
{
	RegisterState<IdleState>();
	RegisterState<MoveState>();
	RegisterState<GuardState>();
	RegisterState<EvadeState>();
	RegisterState<ParryState>();

	// çUåÇÉXÉeÅ[Égìoò^
	//RegisterState<SwordsMan_LightAttack1>();
	//RegisterState<SwordsMan_LightAttack2>();
	//RegisterState<SwordsMan_LightAttack3>();
	//RegisterState<SwordsMan_LightAttack4>();

	//RegisterState<SwordsMan_HeavyAttack1>();
	//RegisterState<SwordsMan_HeavyAttack2>();
	//RegisterState<SwordsMan_HeavyAttack3>();

	//RegisterState<SwordsMan_FinishAttack>();
}

void SwordsMan::OnHit(const HitContext& context)
{
}

void SwordsMan::RequestAttack(void)
{
}
