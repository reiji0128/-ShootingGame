#include "ColliderComponent.h"
#include "Actor.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="owner">Actorのポインタ</param>
/// <param name="type">コライダーのタイプ</param>
/// <param name="updateOrder">更新順序</param>
ColliderComponent::ColliderComponent(Actor* owner, ColliderTypeEnum type, int updateOrder)
    : Component(owner)
    ,mColliderType(type)
{
    // ColliderComponentを持っているActorのタグを取得
    mTag = GetOwner()->GetTag();
}

/// <summary>
/// タグの取得
/// </summary>
/// <returns>衝突したオブジェクトのタグ</returns>
Tag ColliderComponent::GetTag()
{
    return mTag;
}

/// <summary>
/// 衝突情報の初期化
/// </summary>
void ColliderComponent::ClearInfo()
{
    mInfo.mFixVec = Vector3(0, 0, 0);
    mInfo.mCollisionPoint = Vector3(0, 0, 0);
}

// 組み合わせ定義は継承先でoverrideする
// 当たり判定組み合わせを定義しないときは常にfalse返す
bool ColliderComponent::Check(BoxCollider* other)
{
    return false;
}

bool ColliderComponent::Check(WallCollider* other)
{
    return false;
}

bool ColliderComponent::Check(LineCollider* other)
{
    return false;
}
