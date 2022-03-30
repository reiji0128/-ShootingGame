#include "LineCollider.h"
#include "Actor.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "WallCollider.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="owner">所有するActorのポインタ</param>
/// <param name="updateOrder">更新順序/param>
LineCollider::LineCollider(Actor* owner, int updateOrder)
    : ColliderComponent(owner, ColliderTypeEnum::Line, updateOrder)
{
    // ラインコライダーをPhysicsWirldに登録
    GAMEINSTANCE.GetPhysics()->AddCollider(this);
}

/// <summary>
/// デストラクタ
/// </summary>
LineCollider::~LineCollider()
{
    printf("remove LineCollider : [%5d] owner->(0x%p)\n", GetID(), mOwner);
    GAMEINSTANCE.GetPhysics()->RemoveCollider(this);
}

/// <summary>
/// ワールド変換処理
/// </summary>
void LineCollider::OnUpdateWorldTransform()
{
	mWorldLine = mLocalLine;
	if (mWorldLine.mRotatable)
	{
		//線分回転処理
		Quaternion q = mOwner->GetRotation();
		Matrix4 rot = Matrix4::CreateFromQuaternion(q);

		mWorldLine.mLineStart = Vector3::Transform(mWorldLine.mLineStart, rot);
		mWorldLine.mLineEnd = Vector3::Transform(mWorldLine.mLineEnd, rot);
	}

	// 平行移動
	mWorldLine.mLineStart += mOwner->GetPosition();
	mWorldLine.mLineEnd += mOwner->GetPosition();
}

/// <summary>
/// 衝突検出
/// </summary>
/// <param name="other">衝突相手のColliderComponentのポインタ</param>
/// <returns>
/// true  : 衝突検出
/// false : 衝突検出なし
/// </returns>
bool LineCollider::CollisionDetection(ColliderComponent* other)
{
	return other->Check(this);
}

/// <summary>
/// BoxColliderと衝突したか？
/// </summary>
/// <param name="other">衝突相手のBoxColliderのポインタ</param>
/// <returns>衝突したか？</returns>
bool LineCollider::Check(BoxCollider* other)
{
	CollisionInfo info;
	if (Intersect(mWorldLine, other->GetWorldBox(), info))
	{
		SetInfo(info);
		return true;
	}
	return false;
}

/// <summary>
/// WallColliderと衝突したか？
/// </summary>
/// <param name="other">衝突相手のWallColliderのポインタ</param>
/// <returns>衝突したか？</returns>
bool LineCollider::Check(WallCollider* other)
{
	CollisionInfo info;
	if (Intersect(other->GetWall(), mWorldLine, info))
	{
		SetInfo(info);
		return true;
	}
	return false;
}
