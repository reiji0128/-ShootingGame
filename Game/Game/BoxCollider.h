#pragma once

#include "Component.h"
#include "Collision.h"
#include "ColliderComponent.h"
#include "PhysicsWorld.h"
#include "Tag.h"

class BoxCollider : public ColliderComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有するActorのポインタ</param>
	/// <param name="updateOrder">更新順序</param>
	BoxCollider(class Actor* owner, int updateOrder = 100);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BoxCollider();

	/// <summary>
	/// ワールド変換処理(ワールド変換行列更新時に当たり判定ボックスを再計算)
	/// </summary>
	void  OnUpdateWorldTransform() override;

	/// <summary>
	/// 衝突検出
	/// </summary>
	/// <param name="other">衝突相手のColliderComponentのポインタ</param>
	/// <returns>
	/// true  : 衝突検出
	/// false : 衝突検出なし
	/// </returns>
	bool CollisionDetection(ColliderComponent* other) override;

// セッター //
	/// <summary>
	/// あたり判定用境界ボックス(AABB)をセット
	/// </summary>
	/// <param name="box">AABBのインスタンス</param>
	void  SetObjectBox(const AABB& box);

	/// <summary>
	/// 回転を許可するか？
	/// </summary>
	/// <param name="value">許可するか？</param>
	void  SetArrowRotate(bool value) { mRotatable = value; }

// ゲッター //
	/// <summary>
	/// ワールド空間上での境界ボックスを取得
	/// </summary>
	/// <returns></returns>
	const AABB& GetWorldBox() const { return mWorldBox; }


protected:
	/// <summary>
	/// BoxColliderと衝突したか？
	/// </summary>
	/// <param name="other">衝突相手のBoxColliderのポインタ</param>
	/// <returns>衝突したか？</returns>
	bool Check(BoxCollider* other) override;

	/// <summary>
	/// WallColliderと衝突したか？
	/// </summary>
	/// <param name="other">衝突相手のWallColliderのポインタ</param>
	/// <returns>衝突したか？</returns>
	bool Check(WallCollider* other) override;

	/// <summary>
	/// LineColliderと衝突したか？
	/// </summary>
	/// <param name="other">衝突相手のLineColliderのポインタ</param>
	/// <returns>衝突したか？</returns>
	bool Check(LineCollider* other) override;

	// オブジェクト空間（変換前）のボックス
	AABB mObjectBox;

	// ワールド空間に置いた時のボックス
	AABB mWorldBox;

	// 回転を許可するか？
	bool mRotatable;

	// フレンドクラス
	friend class PhysicsWorld;
};