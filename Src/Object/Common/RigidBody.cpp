#include "RigidBody.h"

RigidBody::RigidBody(void)
{
	bodyType_= BodyType::DYNAMIC; // デフォルトは動的剛体
	mass_ = 1.0f; // デフォルトの質量
	inverseMass_ = 1.0f / mass_; // 逆質量の計算
	velocity_ = VGet(0.0f, 0.0f, 0.0f); // 初期速度
	force_ = VGet(0.0f, 0.0f, 0.0f); // 初期力
	angularVelocity_ = VGet(0.0f, 0.0f, 0.0f);
	torque_ = VGet(0.0f, 0.0f, 0.0f);
	useGravity_ = false; // 重力を使用する
	isGrounded_ = false; // 地面に接していない
	useRotation_ = false;
	moveSpeed_ = 1.0f; // 移動速度のデフォルト値
	jumpForce_ = 10.0f; // ジャンプ力のデフォルト値
	maxMoveSpeed_ = 10.0f; // 最大移動速度のデフォルト値
}

RigidBody::~RigidBody(void)
{
}

void RigidBody::SetBodyType(BodyType type)
{
	bodyType_ = type;
	// 静的剛体の場合、逆質量を0に設定
	if (type == BodyType::STATIC)
	{
		inverseMass_ = 0.0f;
		//重力を使用しない
		useGravity_ = false;
	}
}

void RigidBody::SetMass(float mass)
{
	mass_ = mass;
	// 逆質量の計算
	if (mass_ != 0.0f)
	{
		inverseMass_ = 1.0f / mass_;
	}
	else
	{
		inverseMass_ = 0.0f; // 質量が0の場合、逆質量も0に設定
	}
}

void RigidBody::SetUseGravity(bool useGravity)
{
	useGravity_ = useGravity;

}

void RigidBody::SetUseRotation(bool useRota)
{
	useRotation_ = useRota;
}

void RigidBody::SetGrounded(bool isGrounded)
{
	isGrounded_ = isGrounded;
}

void RigidBody::SetVelocity(VECTOR velocity)
{
	velocity_ = velocity;
}

void RigidBody::AddForce(VECTOR force)
{
	force_ = VAdd(force_, force);
}

void RigidBody::SetAngularVelocity(VECTOR angVel)
{
	angularVelocity_ = angVel;
}

void RigidBody::AddTorque(VECTOR torque)
{
	torque_ = VAdd(torque_, torque);
}

void RigidBody::SetMoveSpeed(float speed)
{
	moveSpeed_ = speed;
}

void RigidBody::SetJumpForce(float force)
{
	jumpForce_ = force;
}

void RigidBody::SetMaxMoveSpeed(float maxSpeed)
{
	maxMoveSpeed_ = maxSpeed;
}

void RigidBody::SetCentroid(VECTOR pos)
{
	centroid_ = pos;
}


void RigidBody::Update(void)
{
	if(inverseMass_ == 0.0f)
	{
		return; // 静的剛体は更新しない
	}
	if (useGravity_&&!isGrounded_)
	{
		force_.y -= GRAVITY; // 重力加速度を加える
	}
	else
	{
		velocity_ = VGet(velocity_.x, 0.0f, velocity_.z);
	}
	VECTOR acceleration = force_;
	velocity_ = VAdd(velocity_, acceleration);
	
	//float yKeep = velocity_.y;
	velocity_ = VScale(velocity_, 0.8f);// 摩擦の減衰(0.975fは仮の値)
	isGrounded_ = false;
	force_ = VGet(0.0f, 0.0f, 0.0f); // 力をリセット

	if (useRotation_)
	{
		float invInertia = 2.5f * inverseMass_;

		angularVelocity_ = VAdd(angularVelocity_, VScale(torque_, invInertia));

		angularVelocity_ = VScale(angularVelocity_, 0.95f);
	}
	else
	{
		angularVelocity_ = VGet(0.0f, 0.0f, 0.0f);
	}

//// 2. ★追加：静止摩擦・ハンチング防止（デッドバンド処理）
//	// 回転速度（角速度）の大きさを計算
//	float angularSpeedSq = VDot(angularVelocity_, angularVelocity_);
//	
//	// しきい値（0.01fなど、環境に合わせて調整してください）
//	// 角から落ちたときの激しい回転（大きな値）はスルーし、
//	// 平行に接してガタガタしている微小な回転（小さな値）だけをピンポイントで仕留めます
//	float stopThreshold = 0.05f; 
//
//	if (angularSpeedSq < stopThreshold * stopThreshold)
//	{
//		// 微小なブレとみなして完全に回転をストップさせる
//		angularVelocity_ = VGet(0.0f, 0.0f, 0.0f);
//	}
	torque_ = VGet(0.0f, 0.0f, 0.0f);
}