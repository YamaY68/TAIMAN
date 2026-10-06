#include "MovementSystem.h"

MovementSystem::MovementSystem(void)
    :
	positions(),
	velocities(),
	localTImeScales(),
	idxToEntityID(),
	EntityIDtoIdx()
{
	positions.resize(MAX_ENTITY_COUNT,VGet(0.0f,0.0f,0.0f));
	velocities.resize(MAX_ENTITY_COUNT,VGet(0.0f,0.0f,0.0f));
	localTImeScales.resize(MAX_ENTITY_COUNT, 1.0f);
	idxToEntityID.resize(MAX_ENTITY_COUNT, INVALID_ENTITY_ID);
	EntityIDtoIdx.resize(MAX_ENTITY_COUNT, INVALID_ENTITY_ID);
}

MovementSystem::~MovementSystem(void)
{
}

void MovementSystem::Register(EntityID id, const VECTOR& pos, const VECTOR& vel)
{
	if (activeCount_ >= MAX_ENTITY_COUNT || id >= MAX_ENTITY_COUNT)return;
	uint32_t idx = activeCount_;
	positions[idx] = pos;
	velocities[idx] = vel;
	localTImeScales[idx] = 1.0f;

	idxToEntityID[idx] = id;
	EntityIDtoIdx[id] = idx;

	activeCount_++;
}

void MovementSystem::Unregister(EntityID id)
{
	if (id >= MAX_ENTITY_COUNT)return;
	uint32_t removeIdx = EntityIDtoIdx[id];
	if (removeIdx == INVALID_ENTITY_ID || removeIdx >= activeCount_)return;
	uint32_t lastIdx = activeCount_ - 1;
	if (removeIdx != lastIdx)
	{
		positions[removeIdx] = positions[lastIdx];
		velocities[removeIdx] = velocities[lastIdx];
		localTImeScales[removeIdx] = localTImeScales[lastIdx];

		EntityID lastEntityID = idxToEntityID[lastIdx];
		idxToEntityID[removeIdx] = lastEntityID;
		EntityIDtoIdx[lastEntityID] = removeIdx;

	}
	EntityIDtoIdx[id] = INVALID_ENTITY_ID;
	activeCount_--;
}

void MovementSystem::Update(float worldScale)
{
	VECTOR* vel = velocities.data();
	const float* scale = localTImeScales.data();

	for(uint32_t i=0;i<activeCount_; ++i)
	{
		float effectiveDt = DELTA_TIME * (worldScale * scale[i]);

		vel[i].y += GRAVITY.y * effectiveDt;

		vel[i].x *= 0.99f;
		vel[i].y *= 0.99f;
		vel[i].z *= 0.99f;
	}
}

VECTOR MovementSystem::GetIntendedMove(uint32_t idx, float worldScale) const
{
	float effectiveDt = DELTA_TIME * (worldScale * localTImeScales[idx]);
	return VScale(velocities[idx], effectiveDt);
}

void MovementSystem::SetLocalTImeScale(EntityID id, float scale)
{
	if (id < MAX_ENTITY_COUNT)
	{
		uint32_t idx = EntityIDtoIdx[id];
		if (idx != INVALID_ENTITY_ID && idx < activeCount_)
		{
			localTImeScales[idx] = scale;
		}
	}
}

void MovementSystem::SetVelocity(EntityID id, const VECTOR& vel)
{
	uint32_t idx = EntityIDtoIdx[id];
	if(idx!= INVALID_ENTITY_ID && idx < activeCount_)
	{
		velocities[idx] = vel;
	}
}

const VECTOR* MovementSystem::GetPosition(EntityID id) const
{
    if(id<MAX_ENTITY_COUNT)
	{
		uint32_t idx = EntityIDtoIdx[id];
		if(idx != INVALID_ENTITY_ID && idx < activeCount_)
		{
			return &positions[idx];
		}
	}
	return nullptr;
}
