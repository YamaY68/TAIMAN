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

	//ì«Ç›çûÇ›
	void Load(void);
	//èâä˙âª
	void Init(void);
	//çXêV
	void Update(void);
	//ï`âÊ
	void Draw(void);
	//âï˙
	void Release(void);
	
	void PostUpdate(void);

	BodyType GetBodyType(void) { return bodyType_; }
	VECTOR GetMovePow(void) { return movement_.GetMovePow(); }
protected:
	virtual void SubLoad(void) {};
	virtual void SubInit(void) {};
	virtual void SubUpdate(void) {};
	virtual void SubDraw(void) {};
	virtual void SubRelease(void) {};

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

