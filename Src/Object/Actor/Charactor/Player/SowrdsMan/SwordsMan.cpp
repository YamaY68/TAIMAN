#include "SwordsMan.h"
#include<memory>
#include"../../../../Common/AnimationController.h"
#include"../../../../../Utility/AsoUtility.h"
#include"../State/CommonStates.h"
#include"../../../Camera/CameraManager.h"
#include"Attack/SwordsManAttackHeaders.h"

const std::string ANIM_PATH = "Data/Model/Player/SowrdMan/";

SwordsMan::SwordsMan(CameraManager* cameramanager)
	:PlayerBase(cameramanager)
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
	transform_.pos.y = 60;
	CreateAttack();
	LoadAnimation();
}

void SwordsMan::SubInit(void)
{
	InitAnimation();
	kind_ = ENTITY_KIND::PLAYER;
	bodyType_ = BodyType::DYNAMIC;
	ChangeState<IdleState>();
	followCamera_ = std::make_unique<PlayerFollowVirtualCamera>(&transform_);
	followCamera_->SetCameraKind(CAMERA_KIND::PLAYER_FOLLOW);
	lockOnCamera_ = std::make_unique<LockOnVirtualCamera>(&transform_,nullptr);
	lockOnCamera_->SetCameraKind(CAMERA_KIND::LOCK_ON);
	if (cameraManager_)
	{
		cameraManager_->AddCamera(followCamera_.get());
		cameraManager_->AddCamera(lockOnCamera_.get());
	}
}

void SwordsMan::SubDraw(void)
{
	DrawFormatString(0, 10, 0xffffff, "State:%s", GetCurrentState()->GetName());
	DrawFormatString(0, 20, 0xffffff, "frame:%d", GetCurrentState()->GetStateFrame());
	DrawFormatString(0, 60, 0xffffff, "pos:[%.2f, %.2f, %.2f]", transform_.pos.x, transform_.pos.y, transform_.pos.z);
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
	animationController_->Add((int)ANIMATION_TYPE::IDLE, 15, ANIM_PATH + "Idle.mv1");
	animationController_->Add((int)ANIMATION_TYPE::WALK, 7, ANIM_PATH + "Walk.mv1");
	animationController_->Add((int)ANIMATION_TYPE::RUN, 20, ANIM_PATH + "Run.mv1");
	animationController_->Add((int)ANIMATION_TYPE::GUARD, 15, ANIM_PATH + "Guard.mv1");
	animationController_->Add((int)ANIMATION_TYPE::EVADE, 45, ANIM_PATH + "Evade.mv1");
	animationController_->Add((int)ANIMATION_TYPE::DIVE, 45, ANIM_PATH + "Dive.mv1");
	animationController_->Add((int)ANIMATION_TYPE::PARRY, 45, ANIM_PATH + "Parry.mv1");
	animationController_->Add((int)ANIMATION_TYPE::LIGHT_ATTACK_1, 55, ANIM_PATH + "LightAttack/LightAttack1.mv1");
	animationController_->Add((int)ANIMATION_TYPE::LIGHT_ATTACK_2, 55, ANIM_PATH + "LightAttack/LightAttack2.mv1");
	animationController_->Add((int)ANIMATION_TYPE::LIGHT_ATTACK_3, 55, ANIM_PATH + "LightAttack/LightAttack3.mv1");
	animationController_->Add((int)ANIMATION_TYPE::LIGHT_ATTACK_4, 55, ANIM_PATH + "LightAttack/LightAttack4.mv1");
	animationController_->Add((int)ANIMATION_TYPE::HEAVY_ATTACK_1, 65, ANIM_PATH + "HeavyAttack/HeavyAttack1.mv1");
	animationController_->Add((int)ANIMATION_TYPE::HEAVY_ATTACK_2, 65, ANIM_PATH + "HeavyAttack/HeavyAttack2.mv1");
	animationController_->Add((int)ANIMATION_TYPE::HEAVY_ATTACK_3, 65, ANIM_PATH + "HeavyAttack/HeavyAttack4.mv1");
	animationController_->Add((int)ANIMATION_TYPE::HEAVY_ATTACK_4, 65, ANIM_PATH + "HeavyAttack/HeavyAttack3.mv1");
	animationController_->Add((int)ANIMATION_TYPE::FINISH, 45, ANIM_PATH + "FinishAttack/Finish.mv1");
}

void SwordsMan::InitAnimation(void)
{
	//のちのち外部ファイル等で対応する
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::LIGHT_ATTACK_1, 0.20f, 0.22f, 0.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::LIGHT_ATTACK_1, 0.22f, 1.00f, 1.0f);

	animationController_->AddSpeedRange((int)ANIMATION_TYPE::LIGHT_ATTACK_2, 0.20f, 0.22f, 0.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::LIGHT_ATTACK_2, 0.22f, 1.00f, 1.0f);

	animationController_->AddSpeedRange((int)ANIMATION_TYPE::LIGHT_ATTACK_3, 0.17f, 0.19f, 0.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::LIGHT_ATTACK_3, 0.19f, 1.00f, 1.0f);

	animationController_->AddSpeedRange((int)ANIMATION_TYPE::LIGHT_ATTACK_4, 0.17f, 0.19f, 0.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::LIGHT_ATTACK_4, 0.19f, 1.00f, 1.0f);

	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_1, 0.17f, 0.20f, 0.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_1, 0.22f, 1.00f, 1.0f);

	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_2, 0.18f, 0.20f, 0.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_2, 0.20f, 0.40f, 1.0f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_2, 0.42f, 0.45f, 0.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_2, 0.45f, 1.00f, 1.0f);

	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_3, 0.32f, 0.33f, 0.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_3, 0.38f, 0.39f, 1.0f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_3, 0.39f, 0.42f, 0.1f);

	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_4, 0.30f, 0.33f, 0.2f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_4, 0.34f, 0.40f, 1.0f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_4, 0.40f, 0.43f, 0.2f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_4, 0.43f, 0.50f, 1.0f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::HEAVY_ATTACK_4, 0.50f, 0.53f, 0.1f);

	animationController_->AddSpeedRange((int)ANIMATION_TYPE::FINISH, 0.00f, 0.200f, 1.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::FINISH, 0.20f, 0.214f, 0.1f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::FINISH, 0.214f, 0.450f, 1.3f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::FINISH, 0.50f, 0.900f, 1000.0f);
	animationController_->AddSpeedRange((int)ANIMATION_TYPE::FINISH, 0.90f, 1.000f, 0.1f); // 範囲逆転を修正
}

void SwordsMan::CreateAttack(void)
{
	RegisterState<IdleState>();
	RegisterState<MoveState>();
	RegisterState<GuardState>();
	RegisterState<EvadeState>();
	RegisterState<ParryState>();

	// 攻撃ステート登録
	RegisterState<SwordsMan_LightAttack1>();
	RegisterState<SwordsMan_LightAttack2>();
	RegisterState<SwordsMan_LightAttack3>();
	RegisterState<SwordsMan_LightAttack4>();

	RegisterState<SwordsMan_HeavyAttack1>();
	RegisterState<SwordsMan_HeavyAttack2>();
	RegisterState<SwordsMan_HeavyAttack3>();
	RegisterState<SwordsMan_HeavyAttack4>();

	RegisterState<SwordsMan_BreakAttack>();
	RegisterState<SwordsMan_FinishAttack>();
}

void SwordsMan::OnHit(const HitContext& context)
{
}

void SwordsMan::RequestAttack(void)
{
	// コンボ受付切れの場合は初段へ遷移して早期終了
	if (comboBufferTimer_ <= 0.0f)
	{
		if (isLightAttack_) ChangeState<SwordsMan_LightAttack1>();
		else                ChangeState<SwordsMan_HeavyAttack1>();
		ClearCombo();
		return;
	}

	// 弱攻撃ルート
	if (isLightAttack_)
	{
		if (nextLightAttackType_ == ATTACK_TYPE::LIGHT_ATTACK_2)      ChangeState<SwordsMan_LightAttack2>();
		else if (nextLightAttackType_ == ATTACK_TYPE::LIGHT_ATTACK_3) ChangeState<SwordsMan_LightAttack3>();
		else if (nextLightAttackType_ == ATTACK_TYPE::LIGHT_ATTACK_4) ChangeState<SwordsMan_LightAttack4>();
		else if (nextLightAttackType_ == ATTACK_TYPE::FINISH)         ChangeState<SwordsMan_FinishAttack>();
		ClearCombo();
		return;
	}

	// 強攻撃ルート
	if (nextHeavyAttackType_ == ATTACK_TYPE::HEAVY_ATTACK_2)      ChangeState<SwordsMan_HeavyAttack2>();
	else if (nextHeavyAttackType_ == ATTACK_TYPE::HEAVY_ATTACK_3) ChangeState<SwordsMan_HeavyAttack3>();
	else if (nextHeavyAttackType_ == ATTACK_TYPE::HEAVY_ATTACK_4) ChangeState<SwordsMan_HeavyAttack4>();
	else if (nextHeavyAttackType_ == ATTACK_TYPE::FINISH)         ChangeState<SwordsMan_FinishAttack>();

	ClearCombo();
}
