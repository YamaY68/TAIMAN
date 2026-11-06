#include "TestEnemy.h"
#include"../CommonState/EnemyIdleState.h"
#include"../../../../Common/AnimationController.h"

const std::string ANIM_PATH = "Data/Model/Enemy/TestEnemy/Animation/";

TestEnemy::TestEnemy(void)
{
}

TestEnemy::~TestEnemy(void)
{
}

void TestEnemy::SubLoad(void)
{
	transform_.modelId = MV1LoadModel("Data/Model/Enemy/TestEnemy/TestEnemy.mv1");
	transform_.scl = VGet(0.5f, 0.5f, 0.5f);
	transform_.pos = VGet(0.0f, 60.0f, 200.0f); // プレイヤーの前方に配置
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
}

void TestEnemy::SubInit(void)
{
	InitState();
	kind_ = ENTITY_KIND::ENEMY;
	bodyType_ = BodyType::DYNAMIC;
	canLockOn_ = true;
	hp_ = 100;

	ChangeState<EnemyIdleState>();

}

void TestEnemy::SubUpdate(void)
{
}

void TestEnemy::SubDraw(void)
{
}

void TestEnemy::SubRelease(void)
{
}

void TestEnemy::InitCollider(void)
{
	Collider collider;
	collider.shape = CollisionShape::CAPSULE;
	collider.layer = ColliderLayer::ACTOR;
	collider.mask = ColliderFilter::MakeMask({ ColliderLayer::ACTOR,ColliderLayer::STAGE });
	collider.isActive = true;
	collider.isTrigger = false;
	collider.capsule.radius = 20.0f;
	collider.capsule.localTop = VGet(0.0f, 20.0f, 0.0f);
	collider.capsule.localDown = VGet(0.0f, 0.0f, 0.0f);
	collider.penetrationRatio = 0;
	AddCollider(collider);
}

void TestEnemy::OnHit(const HitContext& context)
{
}

void TestEnemy::InitState(void)
{
}

void TestEnemy::LoadAnimation(void)
{
	animationController_->Add((int)ANIMAITON_TYPE::IDLE, 15, ANIM_PATH + "Idle.mv1");

}

void TestEnemy::InitAnimation(void)
{
}

void TestEnemy::CreateState(void)
{
	RegisterState<EnemyIdleState>();
}
