#pragma once
#include "Actor.h"
#include <vector>
#include "SkeletalMeshComponent.h"
#include "ShaderTag.h"

enum class EnemyState
{
	STATE_IDLE,
	STATE_PATROL,
	STATE_RUN,
	STATE_ATTACK,
	STATE_HIT,
	STATE_DEATH,

	STATE_NUM
};

class EnemyActor : public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="dir">向き</param>
	EnemyActor(const Vector3& pos, const Vector3& dir);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyActor();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(class ColliderComponent* ownCollider, class ColliderComponent* otherCollider) override;

//セッター //
	void SetIsHitTrig(bool hitTrig) { mIsHitTrig = hitTrig; }

//ゲッター //
	SkeletalMeshComponent* GetSkeltalMeshComp() { return mSkelComp; }
	const Animation* GetAnim(EnemyState state) const { return mAnimations[static_cast<unsigned int>(state)]; }
	const bool GetIsBGTrig() const { return mIsBGTrig; }
	const bool GetIsHitTrig() const { return mIsHitTrig; }
	const Vector3& GetVelosity() const { return mVelocity; }
	void SetVelosity(Vector3 velosity) { mVelocity = velosity; }

private:
	class SkeletalMeshComponent* mSkelComp; // メッシュ
	class BoxCollider* mHitBox; // 当たり用ボックス
	class BoxCollider* mBGTrigger; // 背景と当たったら方向転換するためのトリガー
	class LineCollider* mLineCollider; // 足元を調べるコライダー

	class Health* mEnemyHealth; //エネミーの体力

	std::vector<const class Animation*> mAnimations; // アニメーション

	Vector3                             mAimVec; // 目標回転方向
	Vector3                             mVelocity; // 移動スピード
	bool                                mIsOnGround; // 接地しているか 
	bool                                mIsBGTrig; // 背景ボックスとヒットしたか？
	bool                                mIsHitTrig; // アタックオブジェクトとヒットしたか？
	bool                                mRotateNow; // 回転中か
	EnemyState                             mNowState; // 現在のステート
	EnemyState                             mNextState; // 次のステート
	float                               mStateTimer; // ステート内の経過時刻を測る雑用タイマー  

	std::vector<class EnemyStateBase*>     mStatePools;    // ステートクラスプール

	ShaderTag mShaderTag;
};