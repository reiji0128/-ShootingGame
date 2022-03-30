#include "BoxCollider.h"
#include "Game.h"
#include "Actor.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "WallCollider.h"
#include "LineCollider.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="owner">所有するActorのポインタ</param>
/// <param name="updateOrder">更新順序</param>
BoxCollider::BoxCollider(Actor* owner, int updateOrder)
    : ColliderComponent(owner, ColliderTypeEnum::Box, updateOrder)
    , mObjectBox(Vector3::Zero, Vector3::Zero)                    // 初期値は大きさなしのボックスに
    , mWorldBox(Vector3::Zero, Vector3::Zero)
    , mRotatable(true)
{
    // ボックスコライダーをPhysicsWirldに登録
    GAMEINSTANCE.GetPhysics()->AddCollider(this);
}

/// <summary>
/// デストラクタ
/// </summary>
BoxCollider::~BoxCollider()
{
    printf("remove BoxCollider : [%5d] owner->( 0x%p )\n", GetID(), mOwner);
    GAMEINSTANCE.GetPhysics()->RemoveCollider(this);
}

/// <summary>
/// ワールド変換処理(ワールド変換行列更新時に当たり判定ボックスを再計算)
/// </summary>
void BoxCollider::OnUpdateWorldTransform()
{
	// オブジェクト空間のボックスにリセット
	mWorldBox = mObjectBox;

	// スケーリング
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();

	// 回転
	if (mRotatable)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	//平行移動
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();

	mWorldBox.CalcVertex();
}

/// <summary>
/// 衝突検出
/// </summary>
/// <param name="other">衝突相手のColliderComponentのポインタ</param>
/// <returns>
/// true  : 衝突検出
/// false : 衝突検出なし
/// </returns>
bool BoxCollider::CollisionDetection(ColliderComponent* other)
{
	return other->Check(this);
}

/// <summary>
/// あたり判定用境界ボックス(AABB)をセット
/// </summary>
/// <param name="box">AABBのインスタンス</param>
void BoxCollider::SetObjectBox(const AABB& box)
{
	mObjectBox = box;
	mRotatable = box.mIsRotatable;
}

/// <summary>
/// BoxColliderと衝突したか？
/// </summary>
/// <param name="other">衝突相手のBoxColliderのポインタ</param>
/// <returns>衝突したか？</returns>
bool BoxCollider::Check(BoxCollider* other)
{
	AABB otherBox, ownBox;
	otherBox = other->GetWorldBox();
	ownBox = GetWorldBox();
	bool ret = Intersect(otherBox, ownBox);
	if (ret)
	{
		CollisionInfo info;
		Vector3 fixVec;
		calcCollisionFixVec(ownBox, otherBox, fixVec);
		info.mFixVec = fixVec;
		SetInfo(info);
	}
	return ret;
}

/// <summary>
/// WallColliderと衝突したか？
/// </summary>
/// <param name="other">衝突相手のWallColliderのポインタ</param>
/// <returns>衝突したか？</returns>
bool BoxCollider::Check(WallCollider* other)
{
	CollisionInfo info;
	if (Intersect(GetWorldBox(), other->GetWall(), info))
	{
		SetInfo(info);
		return true;
	}
	return false;
}

/// <summary>
/// LineColliderと衝突したか？
/// </summary>
/// <param name="other">衝突相手のLineColliderのポインタ</param>
/// <returns>衝突したか？</returns>
bool BoxCollider::Check(LineCollider* other)
{
	CollisionInfo info;
	if (Intersect(other->GetLine(), GetWorldBox(), info))
	{
		SetInfo(info);
		return true;
	}
	return false;
}
