#include "WallCollider.h"
#include "Collision.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="owner">所有するアクターのポインタ</param>
/// <param name="updateOrder">更新順序</param>
WallCollider::WallCollider(Actor* owner, int updateOrder)
    : ColliderComponent(owner, ColliderTypeEnum::Wall, updateOrder)
    , mWall()
{
    // ウォールコライダーをPhysicsWirldに登録
    GAMEINSTANCE.GetPhysics()->AddCollider(this);
}

/// <summary>
/// デストラクタ
/// </summary>
WallCollider::~WallCollider()
{
    printf("remove WallCollider : [%5d] owner->(0x%p)\n", GetID(), mOwner);
    GAMEINSTANCE.GetPhysics()->RemoveCollider(this);
}

/// <summary>
/// 衝突検出
/// </summary>
/// <param name="other">衝突相手のColliderComponentのポインタ</param>
/// <returns>
/// true  : 衝突検出
/// false : 衝突検出なし
/// </returns>
bool WallCollider::CollisionDetection(ColliderComponent* other)
{
    return other->Check(this);
}

/// <summary>
/// 壁のセット
/// </summary>
/// <param name="wall">壁のインスタンス</param>
void WallCollider::SetWall(const Wall& wall)
{
    // スロープなら当たり判定タイプ変更
    if (wall.mIsSlope)
    {
        ChangeColliderType(ColliderTypeEnum::Slope);
    }
    mWall = wall;
}

/// <summary>
/// 壁のインスタンスの取得
/// </summary>
/// <returns>壁のインスタンス</returns>
const Wall& WallCollider::GetWall() const
{
    return mWall;
}

/// <summary>
/// BoxColliderと衝突したか？
/// </summary>
/// <param name="other">衝突相手のBoxColliderのポインタ</param>
/// <returns>衝突したか？</returns>
bool WallCollider::Check(BoxCollider* other)
{
    CollisionInfo info;
    if (Intersect(other->GetWorldBox(), mWall, info))
    {
        other->SetInfo(info);
        return true;
    }

    return false;
}
