#pragma once
#include "Actor.h"
#include <vector>
#include "SkeletalMeshComponent.h"
#include "Math.h"

// プレイヤーの状態
enum class PlayerState
{
	STATE_IDLE,
	STATE_RUN,
	STATE_DAMAGE,
	STATE_DEATH,

	STATE_NUM
};

class PlayerActor :public Actor
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="scale">スケール</param>
	/// <param name="gpmeshFileName">gpmeshのパス</param>
	/// <param name="gpskelFileName">gpskelのパス</param>
	PlayerActor(const Vector3& pos, const float& scale, const char* gpmeshFileName, const char* gpskelFileName);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerActor();

	/// <summary>
	/// プレイヤーの更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void UpdateActor(float deltaTime)override;

	void ActorInput(const uint8_t* keys)override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="ownCollider"></param>
	/// <param name="otherCollider"></param>
	void OnCollisionEnter(class ColliderComponent* ownCollider, class ColliderComponent* otherCollider)override;

// セッター //
	/// <summary>
	/// 移動スピードのセット
	/// </summary>
	/// <param name="velosity">移動スピード</param>
	void SetVelosity(const Vector3& velosity) { mVelocity = velosity; }

// ゲッター //
	/// <summary>
	/// スケルタルメッシュの取得
	/// </summary>
	/// <returns>スケルタルメッシュコンポーネントのポインタ</returns>
	class SkeletalMeshComponent* GetSkeltalMeshComp() { return mSkeltalMeshComp; }
	
	/// <summary>
	/// アニメーションの取得
	/// </summary>
	/// <param name="state">プレイヤーの状態</param>
	/// <returns>アニメーション</returns>
	const Animation* GetAnim(PlayerState state) const { return mAnimations[static_cast<unsigned int>(state)]; }

private:
	// 移動スピード
	Vector3 mVelocity;

	// ボーンのインデックス
	int mBoneIndex;

	// ボーンの行列
	Matrix4 mBoneMatrix;

	// プレイヤーの現在の状態
	PlayerState mNowState;

	// プレイヤーの次の状態
	PlayerState mNextState;

	// アニメーション可変長コンテナ
	std::vector<const class Animation*> mAnimations;

	// ステートクラスプール
	std::vector<class PlayerStateBase*> mStatePools;

	// スケルタルメッシュコンポーネントのポインタ
	class SkeletalMeshComponent* mSkeltalMeshComp;

	// ボックスコライダーのポインタ
	class BoxCollider* mBoxCollider;

	// ラインコライダーのポインタ
	class LineCollider* mLineCollider;
	
	// MoveComponentクラスのポインタ
	class MoveComponent* mMoveComp;
};
