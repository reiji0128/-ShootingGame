#pragma once

#include "Component.h"
#include "ColliderComponent.h"
#include "PhysicsWorld.h"
#include "Collision.h"

class WallCollider : public ColliderComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有するアクターのポインタ</param>
	/// <param name="updateOrder">更新順序</param>
	WallCollider(class Actor* owner, int updateOrder = 100);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WallCollider();

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
	/// 壁のセット
	/// </summary>
	/// <param name="wall">壁のインスタンス</param>
	void SetWall(const Wall& wall);

// ゲッター //
	/// <summary>
	/// 壁のインスタンスの取得
	/// </summary>
	/// <returns>壁のインスタンス</returns>
	const Wall& GetWall()const;


protected:
	/// <summary>
	/// BoxColliderと衝突したか？
	/// </summary>
	/// <param name="other">衝突相手のBoxColliderのポインタ</param>
	/// <returns>衝突したか？</returns>
	bool Check(BoxCollider* other)override;

	// 壁のインスタンス
	Wall mWall;

	// フレンドクラス
	friend class PhysicsWorld;
};