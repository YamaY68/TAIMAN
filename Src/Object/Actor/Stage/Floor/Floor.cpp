#include "Floor.h"


Floor::Floor(void)
{
}

Floor::~Floor(void)
{
}

void Floor::SubLoad(void)
{
	transform_.modelId = MV1LoadModel("Data/Model/Stage/Stage.mv1");
}

void Floor::SubInit(void)
{
	transform_.scl = VGet(2,0.5f, 2);
	transform_.pos = VGet(0, 0, 0);
	bodyType_ = BodyType::STATIC;
}

void Floor::SubDraw(void)
{
}

void Floor::SubRelease(void)
{
}

void Floor::InitCollider(void)
{
	Collider collider;
	collider.shape = CollisionShape::CUBE;
	collider.layer = ColliderLayer::STAGE;
	collider.mask = ColliderFilter::MakeMask({ ColliderLayer::ACTOR });
	collider.isTrigger = false;
	collider.isActive = true;
	collider.box.halfSize = VGet(1000.0f, 10.0f, 1000.0f);
	AddCollider(collider);
}
