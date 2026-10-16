#pragma once
#include<DxLib.h>
#include<memory>
#include<vector>
#include<map>

#include"../Common/Movement.h"
#include"../Common/Transform.h"

using EntityID = uint32_t;
constexpr EntityID INAVALID_ENTITY_ID = 0xffffffff;
class AnimationController;


class ActorBase
{
public:
	enum class BodyType
	{
		STATIC,
		DYNAMIC,
	};

	ActorBase(void);
	virtual ~ActorBase(void);

	//読み込み
	void Load(void);
	//初期化
	void Init(void);
	//更新
	void Update(void);
	//更新後処理
	void PostUpdate(void);
	//描画
	void Draw(void);
	//解放
	void Release(void);


	BodyType GetBodyType(void) { return bodyType_; }
	VECTOR GetMovePow(void) { return movement_.GetMovePow(); }
protected:
	virtual void SubLoad(void) {};
	virtual void SubInit(void) {};
	virtual void SubUpdate(void) {};
	virtual void SubDraw(void) {};
	virtual void SubRelease(void) {};

	virtual void InitCollider(void) {};

	virtual void LoadAnimation(void) {};

	BodyType bodyType_;
	Movement movement_;
	Transform transform_;
	std::unique_ptr<AnimationController>animationController_;
	int entityID_;
	bool isDraw_;
	bool isActive_;
	float slowTime_;
};

