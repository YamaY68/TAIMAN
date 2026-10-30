#pragma once
#include<vector>
#include<memory>
#include<cstdint>
#include<unordered_map>
#include"../../Actor/EntityKind.h"

class ActorBase;
class CollisionManager;
using EntityID = uint32_t;
constexpr EntityID INAVALID_ENTITY_ID = 0xffffffff;

class ActorManager
{
public:
	static constexpr size_t MAX_ACTORS = 10000;

	ActorManager(void);
	~ActorManager(void);

	//更新
	void Update(void);
	//更新後処理
	void PostUpdate(void);
	//描画
	void Draw(void);
	//解放
	void Release(void);
	/// <summary>
	/// コリジョンマネージャーの設定
	/// </summary>
	/// <param name="colMng"></param>
	void SetCollisionManager(CollisionManager* colMng) { colMng_ = colMng; }
	/// <summary>
	/// 複数アクターの登録
	/// </summary>
	/// <param name="newActors"></param>
	/// <returns></returns>
	std::vector<EntityID> RegisterActors(std::vector<std::unique_ptr<ActorBase>>newActors);
	/// <summary>
	/// アクターの登録
	/// </summary>
	/// <param name="newActor"></param>
	/// <returns></returns>
	EntityID RegisterActor(std::unique_ptr<ActorBase>newActor);
	/// <summary>
	/// アクターの破棄予約
	/// </summary>
	/// <param name="id"></param>
	void DestroyActor(EntityID id);
	//アクター配列の取得
	const std::vector<std::unique_ptr<ActorBase>>& GetActors(void)const { return actors_; }
	//アクティブアクター総数の取得
	size_t GetActiveActorCount(void)const { return activeEntityCount_; }
	/// <summary>
	/// 指定種別のアクターを取得
	/// </summary>
	/// <param name="kind"></param>
	/// <returns></returns>
	ActorBase* FindActorByKind(ENTITY_KIND kind)const;
	/// <summary>
	/// 指定種別のアクター群を取得
	/// </summary>
	/// <param name="kind"></param>
	/// <returns></returns>
	std::vector<ActorBase*>FindActorsByKind(ENTITY_KIND kind)const;
	/// <summary>
	/// IDからアクターを取得
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	ActorBase* GetActor(EntityID id)
	{
		if (id < MAX_ACTORS && actors_[id])return actors_[id].get();
		return nullptr;
	}

private:
	void ProcessPendingDestroys(void);

	std::vector<std::unique_ptr<ActorBase>>actors_;
	std::vector<EntityID>freeEntityIDs_;
	std::vector<EntityID>pendingDestroys_;
	size_t activeEntityCount_ = 0;

	CollisionManager* colMng_ = nullptr;
};

