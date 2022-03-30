#pragma once

#include "Component.h"
#include "ColliderComponent.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "Tag.h"

class LineCollider : public ColliderComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有するActorのポインタ</param>
	/// <param name="updateOrder">更新順序/param>
	LineCollider(class Actor* owner, int updateOrder = 100);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LineCollider();

	/// <summary>
	/// ワールド変換処理
	/// </summary>
	void         OnUpdateWorldTransform() override;

	/// <summary>
	/// 衝突検出
	/// </summary>
	/// <param name="other">衝突相手のColliderComponentのポインタ</param>
	/// <returns>
	/// true  : 衝突検出
	/// false : 衝突検出なし
	/// </returns>
	bool         CollisionDetection(ColliderComponent* other) override;

// セッター //
	/// <summary>
	/// Lineのセット
	/// </summary>
	/// <param name="line">セットするLine</param>
	void         SetLine(const Line& line) { mLocalLine = line; }
	
	/// <summary>
	/// 回転を許可するか？
	/// </summary>
	/// <param name="value">許可するか？</param>
	void         SetArrowRotate(bool value) { mWorldLine.mRotatable = value; }
	
// ゲッター //
	/// <summary>
	/// Lineの取得
	/// </summary>
	/// <returns></returns>
	const Line&  GetLine() const { return mWorldLine; }


protected:
	/// <summary>
	/// BoxColliderと衝突したか？
	/// </summary>
	/// <param name="other">衝突相手のBoxColliderのポインタ</param>
	/// <returns>衝突したか？</returns>
	bool         Check(BoxCollider* other) override;

	/// <summary>
	/// WallColliderと衝突したか？
	/// </summary>
	/// <param name="other">衝突相手のWallColliderのポインタ</param>
	/// <returns>衝突したか？</returns>
	bool         Check(WallCollider* other) override;

	// ローカル位置
	Line         mLocalLine;

	// 線分
	Line         mWorldLine;

	// フレンドクラス
	friend class PhysicsWorld;



};