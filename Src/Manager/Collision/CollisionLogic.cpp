#include "CollisionLogic.h"
#include<algorithm>
#include<cmath>
#include"../../Object/Actor/Collider/Collider.h"

CollisionLogic::CollisionLogic(void)
{
}

CollisionLogic::~CollisionLogic(void)
{
}

CollisionPairType CollisionLogic::GetCollisionPairType(CollisionShape shapeA, CollisionShape shapeB)
{
    if (static_cast<uint8_t>(shapeA) > static_cast<uint8_t>(shapeB))
    {
        std::swap(shapeA, shapeB);
    }
    if (shapeA == CollisionShape::SPHERE && shapeB == CollisionShape::SPHERE)return CollisionPairType::SPHERE_SPHERE;
    if (shapeA == CollisionShape::SPHERE && shapeB == CollisionShape::CAPSULE)return CollisionPairType::SPHERE_CAPSULE;
    if (shapeA == CollisionShape::SPHERE && shapeB == CollisionShape::CUBE)return CollisionPairType::SPHERE_CUBE;
    if (shapeA == CollisionShape::CAPSULE && shapeB == CollisionShape::CAPSULE)return CollisionPairType::CAPSULE_CAPSULE;
    if (shapeA == CollisionShape::CAPSULE && shapeB == CollisionShape::CUBE)return CollisionPairType::CAPSULE_CUBE;
    if (shapeA == CollisionShape::CUBE && shapeB == CollisionShape::CUBE)return CollisionPairType::CUBE_CUBE;
    return CollisionPairType::IDLE;
}

CollisionResult CollisionLogic::DispatchCollision(const CollisionPairType pairType, const Collider* colliderA, const Collider* colliderB)
{
    bool needSwap = static_cast<uint8_t>(colliderA->shape) > static_cast<uint8_t>(colliderB->shape);
    const Collider* a = needSwap ? colliderB : colliderA;
    const Collider* b = needSwap ? colliderA : colliderB;
    CollisionResult result;
    switch (pairType)
    {
    case CollisionPairType::SPHERE_SPHERE:
        result = SphereToSphere(const_cast<Collider*>(a), const_cast<Collider*>(b));
        break;
    case CollisionPairType::SPHERE_CUBE:
        result  = SphereToCube(const_cast<Collider*>(a), const_cast<Collider*>(b));
        break;
    case CollisionPairType::SPHERE_CAPSULE:
        result = SphereToCapsule(const_cast<Collider*>(a), const_cast<Collider*>(b));
        break;
    case CollisionPairType::CAPSULE_CAPSULE:
        result = CapsuleToCapsule(const_cast<Collider*>(a), const_cast<Collider*>(b));
        break;
    case CollisionPairType::CAPSULE_CUBE:
        result = CapsuleToCube(const_cast<Collider*>(a), const_cast<Collider*>(b)); 
        break;
    case CollisionPairType::CUBE_CUBE:
        result = CubeToCube(const_cast<Collider*>(a), const_cast<Collider*>(b));
        break;
    default:
        break;
    }
    if (needSwap && result.isHit)
    {
        result.normal = VScale(result.normal, -1);
    }
    return result;
}

CollisionResult CollisionLogic::SphereToSphere(Collider* a, Collider* b)
{
    CollisionResult result;
    VECTOR posA = a->GetWorldPos();
    VECTOR posB = b->GetWorldPos();
    float rA = a->sphere.radius;
    float rB = b->sphere.radius;
    
    VECTOR diff = VSub(posA, posB);
    float distSq = VDot(diff, diff);
    float rSum = rA + rB;
    //当たっていない
    if (distSq > rSum*rSum)return result;
    result.isHit = true;
    //ほぼ同じ位置
    if (distSq < 0.001f)
    {
        result.normal = VGet(0.0f, 1.0f, 0.0f);
        result.penetration = rSum;
        result.hitPoint = posA;
    }
    else
    {
        float dist = std::sqrt(distSq);
        result.normal = VScale(diff, 1.0f / dist);
        result.penetration = rSum - dist;
        result.hitPoint = VSub(posA, VScale(result.normal, rA - result.penetration * 0.5f));
    }
    return result;
}

CollisionResult CollisionLogic::SphereToCapsule(Collider* a, Collider* b)
{
    CollisionResult result;

    VECTOR sPos = a->GetWorldPos();
    float sRadius = a->sphere.radius;

    VECTOR capPos = b->GetWorldPos();
    VECTOR pTop = VAdd(capPos, b->capsule.localTop);
    VECTOR pDown = VAdd(capPos, b->capsule.localDown);
    VECTOR seg = VSub(pTop, pDown);

    float segLenSq = VDot(seg, seg);
    float t = 0.0f;
    //球の中心からカプセルの線分への射影
    if (segLenSq > 0.001f)
    {
        t = VDot(VSub(sPos, pDown), seg) / segLenSq;
        t = std::clamp(t, 0.0f, 1.0f);
    }
    //球中心からカプセルへの最接近点
    VECTOR nearestOnSeg = VAdd(pDown, VScale(seg, t));
	//nearestOnSegを中心とする球と球の衝突判定
    VECTOR diff = VSub(sPos, nearestOnSeg);
    float distSq = VDot(diff, diff);
    float rSum = sRadius + b->capsule.radius;
    //当たっていない
    if (distSq > rSum * rSum)return result;
    result.isHit = true;
	//ほぼ同じ位置
    if (distSq < 0.0001f)
    {
        result.normal = VGet(0.0f, 1.0f, 0.0f);
        result.penetration = rSum;
        result.hitPoint = sPos;
    }
    else
    {
        float dist = std::sqrt(distSq);
        result.normal = VScale(diff, 1.0f / dist);
        result.penetration = rSum - dist;
        result.hitPoint = VAdd(nearestOnSeg, VScale(result.normal, b->capsule.radius));
    }
    return result;
}

CollisionResult CollisionLogic::SphereToCube(Collider* a, Collider* b)
{
    //ひとまず回転無しで計算

    CollisionResult result;
    VECTOR sPos = a->GetWorldPos();
    float sRadius = a->sphere.radius;

    VECTOR bPos = b->GetWorldPos();
    VECTOR half = b->box.halfSize;
    //箱の中心を原点としたときの相対的な球の中心位置
    VECTOR localDiff = VSub(sPos, bPos);
    //球の中心位置をクランプして箱の表面に投影
    VECTOR clamped =
    {
        std::clamp(localDiff.x,-half.x,half.x),
        std::clamp(localDiff.y,-half.y,half.y),
        std::clamp(localDiff.z,-half.z,half.z) };
    //ワールド空間における箱の最接近点
    VECTOR closestPoint = VAdd(bPos, clamped);
	//球の中心から箱の最接近点へのベクトル
    VECTOR diff = VSub(sPos, closestPoint);
    float distSq = VDot(diff, diff);
	//当たっていない
    if (distSq > sRadius * sRadius)return result;

    result.isHit = true;
    //球の中心が箱の中にある場合
    if (distSq < 0.0001f)
    {
        // 中心が箱内部にある場合：X, Y, Z のうち最も外側（壁）に近い面を求めて脱出方向を決める
        float dx = half.x - std::abs(localDiff.x);
        float dy = half.y - std::abs(localDiff.y);
        float dz = half.z - std::abs(localDiff.z);

        if (dx <= dy && dx <= dz)
        {
            result.normal = VGet(localDiff.x >= 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);
            result.penetration = dx + sRadius;
        }
        else if (dy <= dx && dy <= dz)
        {
            result.normal = VGet(0.0f, localDiff.y >= 0.0f ? 1.0f : -1.0f, 0.0f);
            result.penetration = dy + sRadius;
        }
        else
        {
            result.normal = VGet(0.0f, 0.0f, localDiff.z >= 0.0f ? 1.0f : -1.0f);
            result.penetration = dz + sRadius;
        }
        result.hitPoint = closestPoint;
    }
    else
    {
        float dist = std::sqrt(distSq);
        result.normal = VScale(diff, 1.0f / dist);
        result.penetration = sRadius - dist;
        result.hitPoint = closestPoint;
    }
    return result;
}

CollisionResult CollisionLogic::CapsuleToCapsule(Collider* a, Collider* b)
{
    CollisionResult result;

    VECTOR posA = a->GetWorldPos();
    VECTOR segStartA = VAdd(posA, a->capsule.localDown);
    VECTOR segEndA = VAdd(posA, a->capsule.localTop);

    VECTOR posB = b->GetWorldPos();
    VECTOR segStartB = VAdd(posB, b->capsule.localDown);
    VECTOR segEndB = VAdd(posB, b->capsule.localTop);

    VECTOR segDirA = VSub(segEndA, segStartA);
    VECTOR segDirB = VSub(segEndB, segStartB);
    VECTOR offset = VSub(segStartA, segStartB);

    float aLen = VDot(segDirA, segDirA);
    float bLen = VDot(segDirB, segDirB);
	float dotDirAB = VDot(segDirA, segDirB);  //AとBの向きの一致度　０なら直角
    float c = VDot(segDirA, offset);      //BからみたAの位置
	float f = VDot(segDirB, offset);      //AからみたBの位置
    //カプセルABと直交する線分の連立方程式
    //aLen*ratioA-dotDirAB*ratioB=-c
    //dotDirAB*ratioA-bLen*ratioB=-f
    // 
    //クラメルの公式
    //Ax=b x=A^-1b
    //aLen  -dotDirAB   ratioA  -c
    //dotDirAB  -bLen   ratioB  -f
    //       A            x     b
    //
    // M＝(  pq)  M^-1=   1/ps-qr(s   -q)
    //    (  rs)                 (-r   p)
    // ps-qt Determinant 行列式
    // A^-1
    //                  1                    (-bLen    -(-dotDirAB))
    //  --------------------------------     (-(dotDirAB)  aLen    )
    // aLen*(-Blen)-(-dotDirAB)*dotDirAB
    //
    // den=-aLenBLen+dotDirAB^2
    // -den=aLenBLen-dotDirAB^2
    float den = aLen * bLen - dotDirAB * dotDirAB;  //分母(Denominator)の計算、クラメルの公式
    //線分１の比率と線分２の比率
    float ratioA = 0.0f;
    float ratioB = 0.0f;
    //
    // |ratioA|       1   |bLen   -dotDirAB||-c|
    // |      |   =-------|                ||  |
    // |ratioB|     den   |dotDirAB   -aLen||-f|
    // 
    //          1   
    // ratioA=----- (bLen*(-c)+(-dotDirAB)*(-f))
    //         den  
    //  -c*bLen+dotDirAB*f
    // =------------------
    //          den
    //   dotDirAB*f-c*BLen
    // ==-----------------
    //          den
    //ratioAを仮決め、カプセルAの線分内にクランプ
    if (den != 0.0f)
    {
        ratioA = std::clamp((dotDirAB * f - c * bLen) / den, 0.0f, 1.0f);
    }
    //ratioAに一番近いカプセルＢ上のratioBを求める
    ratioB = (bLen > 0.0001f) ? ((dotDirAB * ratioA + f) / bLen) : 0.0f;
    //ratioBがカプセルＢの端からはみ出てる場合
    if (ratioB < 0.0f)
    {
        //末端p2に固定
        ratioB = 0.0f;
        //p2からカプセルＡの棒へ向けて、最接近点を計算
        ratioA = (aLen > 0.0f) ? std::clamp(-c / aLen, 0.0f, 1.0f) : 0.0f;
    }
    else if (ratioB > 1.0f)
    {
        //末端q2に固定
        ratioB = 1.0f;
        //q2からカプセルＡの棒へ向けて、最接近点を計算
        ratioA = (aLen > 0.0f) ? std::clamp((dotDirAB - c) / aLen, 0.0f, 1.0f) : 0.0f;
    }
    //比率からワールド空間の最接近点を計算
    VECTOR cA = VAdd(segStartA, VScale(segDirA, ratioA));
    VECTOR cB = VAdd(segStartB, VScale(segDirB, ratioB));
    //球・球の衝突計算
    VECTOR diff = VSub(cA, cB);
    float distSq = VDot(diff, diff);
    float rSum = a->capsule.radius + b->capsule.radius;

    if (distSq > rSum * rSum)return result;

    result.isHit = true;
    if (distSq < 0.0001f)
    {
        result.normal = VGet(0.0f, 1.0f, 0.0f);
        result.penetration = rSum;
        result.hitPoint = cA;
    }
    else
    {
        float dist = std::sqrt(distSq);
        result.normal = VScale(diff, 1.0f / dist);
        result.penetration = rSum - dist;
        result.hitPoint = VSub(cA, VScale(result.normal, a->capsule.radius - result.penetration * 0.5f));
    }
    return result;
}

CollisionResult CollisionLogic::CapsuleToCube(Collider* a, Collider* b)
{
    CollisionResult result;

    VECTOR capPos = a->GetWorldPos();
    VECTOR pTop = VAdd(capPos, a->capsule.localTop);
    VECTOR pDown = VAdd(capPos, a->capsule.localDown);
    float radius = a->capsule.radius;
    VECTOR bPos = b->GetWorldPos();
    VECTOR half = b->box.halfSize;
    const int splitCount = 4;       //線分を４等分

    float maxPen = -1.0f;       //最も深かっためり込み量
    VECTOR bestNormal = VGet(0, 1, 0);
    VECTOR bestHitPoint = bPos;

    //線分を等間隔に分割し、複数の球として直方体と判定する
    for (int i = 0; i <= splitCount; ++i)
    {
        //t=0,0.25,0.5,0.75,1
        float t = static_cast<float>(i) / static_cast<float>(splitCount);
        VECTOR pt = VAdd(pDown, VScale(VSub(pTop, pDown), t));

        VECTOR localDiff = VSub(pt, bPos);
        VECTOR clamped = {
            std::clamp(localDiff.x,-half.x,half.x),
            std::clamp(localDiff.y,-half.y,half.y),
            std::clamp(localDiff.z,-half.z,half.z) };
        VECTOR closestBoxPt = VAdd(bPos, clamped);

        VECTOR diff = VSub(pt, closestBoxPt);
        float distSq = VDot(diff, diff);
        if (distSq <= radius * radius)
        {
            float dist = std::sqrt(distSq);
            float pen = radius - dist;
            if (pen > maxPen)
            {
                maxPen = pen;
                bestNormal = (dist > 0.0001f) ? VScale(diff, 1.0f / dist) : VGet(0, 1, 0);
                bestHitPoint = closestBoxPt;
            }
        }
    }
    if (maxPen > 0.0f)
    {
        result.isHit = true;
        result.normal = bestNormal;
        result.penetration = maxPen;
        result.hitPoint = bestHitPoint;
    }
    return result;
}

CollisionResult CollisionLogic::CubeToCube(Collider* a, Collider* b)
{
    //回転を考慮しない
    CollisionResult result;
    VECTOR posA = a->GetWorldPos();
    VECTOR posB = b->GetWorldPos();
    VECTOR halfA = a->box.halfSize;
    VECTOR halfB = b->box.halfSize;

    VECTOR delta = VSub(posA, posB);
    //各軸の重なり量
    VECTOR overLap = {
        (halfA.x + halfB.x) - std::fabs(delta.x),
        (halfA.y + halfB.y) - std::fabs(delta.y),
        (halfA.z + halfB.z) - std::fabs(delta.z) };
    //すべての軸が重なっていない
    if (overLap.x <= 0.0f || overLap.y <= 0.0f || overLap.z <= 0.0f)
    {
        return result;
    }

    result.isHit = true;
    //重なり量が一番少ない軸で判定
    if (overLap.x <= overLap.y && overLap.x <= overLap.z)
    {
        result.penetration = overLap.x;
        result.normal = VGet(delta.x > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);
    }
    else if (overLap.y <= overLap.x && overLap.y <= overLap.z)
    {
        result.penetration = overLap.y;
        result.normal = VGet(0.0f, delta.y > 0.0f ? 1.0f : -1.0f, 0.0f);
    }
    else
    {
        result.penetration = overLap.z;
        result.normal = VGet(0.0f, 0.0f, delta.z > 0.0f ? 1.0f : -1.0f);
    }
    result.hitPoint = VScale(VAdd(posA, posB), 0.5f);
    return result;
}
