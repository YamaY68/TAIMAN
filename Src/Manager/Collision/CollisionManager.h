#pragma once
#include<DxLib.h>
#include<vector>
#include"../../Object/Common/Transform.h"
#include"../../Object/Actor/Collider/Collider.h"
#include"../../Object/Actor/Collider/HitContext.h"
#include"CollisionInfo.h"

class ActorBase;




class CollisionManager
{
public:
	static constexpr int SOLVE_ITERATIONS = 3; // 衝突解決の反復回数

	CollisionManager(void);
	~CollisionManager(void);
	void RegisterActor(ActorBase* actor, uint32_t ownerID);
	void UnregisterActor(uint32_t ownerID);
	void Clear(void);
	void Update(void);

private:
	struct ContactRecord
	{
		size_t indexA;
		size_t indexB;
		VECTOR hitPoint;
		bool isTrigger;
	};
	/// <summary>
	/// 位置の押し出し
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="normal"></param>
	/// <param name="penetration"></param>
	void Resolve(ActorBase* a, ActorBase* b, const VECTOR& normal, float penetration);
	/// <summary>
	/// 衝突可能か
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="iter"></param>
	/// <param name="outIsTriggerPair"></param>
	/// <returns></returns>
	bool CanCollide(const ColliderEntry& a, const ColliderEntry& b, int iter, bool& outIsTriggerPair)const;
	/// <summary>
	/// ペアの衝突判定、押し出し、接触記録
	/// </summary>
	/// <param name="indexA"></param>
	/// <param name="indexB"></param>
	/// <param name="iter"></param>
	/// <param name="isTriggerPair"></param>
	/// <param name="outContacts"></param>
	/// <returns></returns>
	bool ProcessPair(size_t indexA, size_t indexB, int iter, bool isTriggerPair, std::vector<ContactRecord>& outContacts);
	/// <summary>
	/// 一括ディスパッチ
	/// </summary>
	/// <param name="contacts"></param>
	void DispatchHitEvents(const std::vector<ContactRecord>& contacts);

	std::vector<ColliderEntry> entries_;
};

