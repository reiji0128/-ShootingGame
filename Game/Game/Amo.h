#pragma once
#include "Actor.h"
#include "Gun.h"

class Amo : public Actor
{
public:
	Amo(Tag tag, Gun* gun);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Amo(Vector3& position,Tag tag,Gun* gun);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Amo();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="deltaTiem">1フレームの経過時間</param>
	void UpdateActor(float deltaTime)override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="ownCollider"></param>
	/// <param name="otherCollider"></param>
	void OnCollisionEnter(class ColliderComponent* ownCollider, class ColliderComponent* otherCollider)override;

	void ActiveAmo(Amo* amo);
private:

	void CreateAmo();

	// 生成数
	const int mCreateNum;

	// 最大アクティブ数
	const int mMaxActive;

	// アクティブ数
	int mActiveCount;

	// アクティブかどうかのフラグ
	bool mIsActive;

	// 弾の動的配列
	std::vector<Amo*> mAmoList;

	// Gunクラスのポインタ
	Gun* mGun;
};