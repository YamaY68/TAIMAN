#pragma once
#include<vector>
#include<cstdint>
#include<DxLib.h>



class MovementSystem
{
public:

	using EntityID = uint32_t;
	static constexpr EntityID INVALID_ENTITY_ID = 0xffffffff;
	static constexpr size_t MAX_ENTITY_COUNT = 10000;
	static constexpr float DELTA_TIME = 1.0f / 60.0f;
	static constexpr VECTOR GRAVITY = { 0.0f,-9.8f,0.0f };

	std::vector<VECTOR>positions;
	std::vector<VECTOR>velocities;
	std::vector<float>localTImeScales;
	std::vector<EntityID>idxToEntityID;
	std::vector<uint32_t>EntityIDtoIdx;

	MovementSystem(void);
	~MovementSystem(void);

	void Register(EntityID id, const VECTOR& pos, const VECTOR& vel = { 0.0f,0.0f,0.0f });
	void Unregister(EntityID id);
	void Update(float worldScale);

	VECTOR GetIntendedMove(uint32_t idx, float worldScale)const;
	uint32_t GetActiveCount(void)const { return activeCount_; }
	void SetLocalTImeScale(EntityID id, float scale);
	void SetVelocity(EntityID id, const VECTOR& vel);
	const VECTOR* GetPosition(EntityID id)const;

private:

	uint32_t activeCount_ = 0;

};

