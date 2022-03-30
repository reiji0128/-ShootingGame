#pragma once

#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "Tag.h"

class BoxCollider;
class WallCollider;
class LineCollider;

/// <summary>
/// コライダータイプ
/// </summary>
enum class ColliderTypeEnum
{
	Box,
	Wall,
	MoveFloor,
	Slope,
	Sphere,
	Line,
};

class ColliderComponent : public Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">Actorのポインタ</param>
	/// <param name="type">コライダーのタイプ</param>
	/// <param name="updateOrder">更新順序</param>
	ColliderComponent(class Actor* owner, ColliderTypeEnum type, int updateOrder = 100);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ColliderComponent() {};
	
// セッター //
	/// <summary>
	/// 衝突情報のセット
	/// </summary>
	void SetInfo(const CollisionInfo& info) { mInfo = info; }

	/// <summary>
	/// コライダータイプの変更
	/// </summary>
	/// <param name="value">コライダータイプ</param>
	void ChangeColliderType(ColliderTypeEnum value) { mColliderType = value; }

// ゲッター //
	/// <summary>
	/// タグの取得
	/// </summary>
	/// <returns>衝突したオブジェクトのタグ</returns>
	Tag GetTag();

	/// <summary>
	/// コライダータイプの取得
	/// </summary>
	/// <returns>コライダータイプ</returns>
	ColliderTypeEnum GetColliderType() { return mColliderType; }

	/// <summary>
	/// 衝突情報の取得
	/// </summary>
	/// <returns>衝突情報</returns>
	CollisionInfo& GetCollisionInfo() { return mInfo; }

	/// <summary>
	/// 衝突情報の初期化
	/// </summary>
	void ClearInfo();

	/// <summary>
	/// ワールド変換処理
	/// </summary>
	virtual void OnUpdateWorldTransform() {};
	
	/// <summary>
	/// Double-Dispatch パターンを使用して衝突判定組み合わせを行う
	/// </summary>
	/// <param name="other">衝突相手のColliderComponentのポインタ</param>
	/// <returns>
	/// true  : 衝突検出
	/// false : 衝突検知なし
	/// </returns>
	virtual bool CollisionDetection(ColliderComponent* other) = 0;

	// Double-Dispatch パターン：Collider同士のふるまいを定義
	virtual bool Check(class BoxCollider* other);
	virtual bool Check(class WallCollider* other);
	virtual bool Check(class LineCollider* other);

protected:
	// オブジェクトのタグ
	Tag mTag;

	// 衝突タイプ
	ColliderTypeEnum mColliderType;

	// 衝突情報
	CollisionInfo mInfo;

	// フレンドクラス
	friend class PhysicsWorld;
};