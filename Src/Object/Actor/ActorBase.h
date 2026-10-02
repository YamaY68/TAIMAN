#pragma once
#include<DxLib.h>
#include<memory>
#include<vector>
#include<map>

#include"../Common/Transform.h"
#include"../Common/RigidBody.h"

class AnimationController;
class ColliderBase;

class ActorBase
{
public:
	ActorBase(void);
	virtual ~ActorBase(void);

	//“Ç‚İ‚İ
	void Load(void);
	//‰Šú‰»
	void Init(void);
	//XV
	void Update(void);
	//•`‰æ
	void Draw(void);
	//‰ğ•ú
	void Release(void);

protected:
	virtual void SubLoad(void) {};
	virtual void SubInit(void) {};
	virtual void SubUpdate(void) {};
	virtual void SubDraw(void) {};
	virtual void SubRelease(void) {};

	virtual void LoadAnimation(void) {};


};

