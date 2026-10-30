#pragma once
#include<DxLib.h>
#include<memory>
#include<vector>
#include<map>

#include"../Common/Movement.h"
#include"../Common/Transform.h"
#include"Collider/Collider.h"
#include"Collider/HitContext.h"
#include"EntityKind.h"
#include"Common/StateMachine.h"

using EntityID = uint32_t;
class AnimationController;


class ActorBase
{
public:
	enum class BodyType
	{
		STATIC,
		DYNAMIC,
	};
	//コンストラクタ
	ActorBase(void);
	//デストラクタ
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
	//トランスフォームの取得
	Transform& GetTransform(void) { return transform_; }
	//ボディタイプの取得
	BodyType GetBodyType(void) { return bodyType_; }
	//移動処理クラスの取得
	Movement& GetMoveMent(void) { return movement_; }
	/// <summary>
	/// コライダー登録
	/// </summary>
	/// <param name="collider">登録するコライダー</param>
	void AddCollider(Collider collider);
	//コライダー群の取得
	std::vector<Collider>& GetColliders(void) { return colliders_; }
	//エンティティ種類の取得
	ENTITY_KIND GetEntityKind(void) { return kind_; }
	//アニメーションコントローラーの取得
	AnimationController* GetAnimationController(void)const { return animationController_.get(); }
	/// <summary>
	/// オブジェクト間の衝突処理
	/// </summary>
	/// <param name="context"></param>
	void OnCollision(const HitContext& context);
	/// <summary>
	/// ステートの登録
	/// </summary>
	/// <typeparam name="S">型名</typeparam>
	/// <typeparam name="...Args">引数型</typeparam>
	/// <param name="...args">引き数</param>
	template <typename S, typename... Args>
	void RegisterState(Args&&... args)
	{
		stateMachine_.RegisterState<S>(std::forward<Args>(args)...);
	}
	//ステートの遷移
	template <typename S>
	void ChangeState()
	{
		stateMachine_.ChangeState<S>();
	}
	//現在ステートの取得
	IState<ActorBase>* GetCurrentState() const { return stateMachine_.GetCurrentState(); }
	//現在ステート名の取得
	const char* GetCurrentStateName() const { return stateMachine_.GetCurrentStateName(); }
	/// <summary>
	/// ステートの取得
	/// </summary>
	/// <typeparam name="S">型名</typeparam>
	/// <param name=""></param>
	/// <returns></returns>
	template<typename S>
	S* GetState(void)const { return stateMachine_.GetState<S>(); }
protected:
	virtual void SubLoad(void) {};
	virtual void SubInit(void) {};
	virtual void SubUpdate(void) {};
	virtual void SubDraw(void) {};
	virtual void SubRelease(void) {};

	virtual void InitCollider(void) {};
	virtual void LoadAnimation(void) {};
	virtual void InitAnimation(void) {};

	virtual void OnHit(const HitContext& context) {};

	BodyType bodyType_ = BodyType::STATIC;
	Movement movement_;
	Transform transform_;
	std::vector<Collider> colliders_;

	std::unique_ptr<AnimationController> animationController_ = nullptr;
	StateMachine<ActorBase> stateMachine_{ this };

	int entityID_ = 0;
	ENTITY_KIND kind_ = ENTITY_KIND::NONE;
	bool isDraw_ = true;
	bool isActive_ = true;
	float slowTime_ = 1.0f;
};