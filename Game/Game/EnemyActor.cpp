#include "EnemyActor.h"
#include "Animation.h"
#include "Mesh.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "LineCollider.h"

#include "EnemyStateIdle.h"
#include "EnemyStatePatrol.h"
#include "EnemyStateRun.h"
#include "EnemyStateAttack.h"
#include "EnemyStateHit.h"
#include "EnemyStateDeath.h"

EnemyActor::EnemyActor(const Vector3& pos, const Vector3& dir)
	:Actor(Tag::Enemy)
	, mSkelComp(nullptr)
	, mHitBox(nullptr)
	, mRotateNow(false)
	, mBGTrigger(nullptr)
	, mLineCollider(nullptr)
	, mIsOnGround(true)
	, mIsBGTrig(false)
	, mIsHitTrig(false)
	, mNowState(EnemyState::STATE_IDLE)
	, mNextState(EnemyState::STATE_PATROL)
	, mAimVec(0.0f, 0.0f, 0.0f)
	, mVelocity(0.0f, 0.0f, 0.0f)
	, mStateTimer(0.0f)
	, mEnemyHealth(nullptr)
{
	mPosition = pos;
	mDirection = dir;

	// メッシュの読み込み
	Mesh* mesh = RENDERER->GetMesh("Assets/Enemy/BeholderSK.gpmesh");
	mSkelComp = new SkeletalMeshComponent(this);
	mSkelComp->SetMesh(mesh);

	// スケルトンの読み込み
	mSkelComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Enemy/BeholderSK.gpskel"));

	// アニメーションの取得 & アニメーション配列にセット
	mAnimations.resize(static_cast<unsigned int>(EnemyState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_IDLE)] = RENDERER->GetAnimation("Assets/Enemy/Beholder_IdleAnim.gpanim", true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_PATROL)] = RENDERER->GetAnimation("Assets/Enemy/Beholder_RunAnim.gpanim", true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_RUN)] = RENDERER->GetAnimation("Assets/Enemy/Beholder_RunAnim.gpanim", true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_ATTACK)] = RENDERER->GetAnimation("Assets/Enemy/Beholder_AttackAnim.gpanim", false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_HIT)] = RENDERER->GetAnimation("Assets/Enemy/Beholder_DieAnim.gpanim", false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_DEATH)] = RENDERER->GetAnimation("Assets/Enemy/Beholder_DieAnim.gpanim", false);


	// アイドル状態のアニメーションをセット
	mSkelComp->PlayAnimation(mAnimations[static_cast<unsigned int>(EnemyState::STATE_IDLE)]);

	// アクターステートプールの初期化
	mStatePools.push_back(new EnemyStateIdle);
	mStatePools.push_back(new EnemyStatePatrol);
	mStatePools.push_back(new EnemyStateRun);
	mStatePools.push_back(new EnemyStateAttack);
	mStatePools.push_back(new EnemyStateHit);
	mStatePools.push_back(new EnemyStateDeath);

	// 当たり判定のセット
	AABB box = mesh->GetCollisionBox();
	box.Scaling(1.0f, 1.0f, 1.0f);
	box.mMin.z = 30.0f;
	box.mIsRotatable = true;
	mHitBox = new BoxCollider(this);
	mHitBox->SetObjectBox(box);

	// 背景トリガー
	AABB enemyForwardBox;
	enemyForwardBox.mMin.x = box.mMax.x;
	enemyForwardBox.mMin.y = box.mMin.y;
	enemyForwardBox.mMin.z = box.mMin.z + 20.0f;
	enemyForwardBox.mMax.x = enemyForwardBox.mMin.x + 30.0f;
	enemyForwardBox.mMax.y = enemyForwardBox.mMin.y + 60.0f;
	enemyForwardBox.mMax.z = enemyForwardBox.mMin.z + 30.0f;
	enemyForwardBox.mIsRotatable = true;
	mBGTrigger = new BoxCollider(this);
	mBGTrigger->SetObjectBox(enemyForwardBox);

	//足元を調べるコライダー
	mLineCollider = new LineCollider(this);
	Line footline(Vector3(0.0f, 0.0f, 100.0f), Vector3(0.0f, 0.0f, -20.0f));
	mLineCollider->SetLine(footline);

	//体力のセット
	//mEnemyHealth = new Health(10.0f, 0.0f, 10.0f);
}

EnemyActor::~EnemyActor()
{
}

void EnemyActor::UpdateActor(float deltaTime)
{
	// ステート外部からステート変更があったか?
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// ステート実行
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// ステート内部から変更があったか?
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	// いずれかの移動処理で得た速度で現在位置更新
	mPosition += mVelocity;
	SetComputeWorldTransform();

	// 次のフレームでトリガーがヒットしているかを調べるためにリセット
	mIsOnGround = false;
	mIsBGTrig = false;
}

void EnemyActor::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
	Tag colliderTag = otherCollider->GetTag();

	//衝突情報
	CollisionInfo info;

	// 背景と衝突した?
	if (colliderTag == Tag::BackGround)
	{
		// 衝突背景はBox?
		if (otherCollider->GetColliderType() == ColliderTypeEnum::Box)
		{
			// 背景Boxに衝突したのはLine?
			if (ownCollider == mLineCollider)
			{
				//落下しているときのみ判定
				if (mVelocity.z < 0.0f)
				{
					info = ownCollider->GetCollisionInfo();
					mPosition = info.mCollisionPoint;
					mIsOnGround = true;
				}
			}

			// 背景コライダー
			if (ownCollider == mBGTrigger)
			{
				mIsBGTrig = true;
			}

			// 足元コライダー
			if (ownCollider == mLineCollider)
			{
				info = ownCollider->GetCollisionInfo();
				mPosition = info.mCollisionPoint;
				mIsOnGround = true;
				ComputeWorldTransform();
			}

			// 背景Boxに衝突したのもBox？
			if (ownCollider->GetColliderType() == ColliderTypeEnum::Box)
			{
				//ヒットボックス？
				if (ownCollider == mHitBox)
				{
					info = ownCollider->GetCollisionInfo();

					mPosition += info.mFixVec;
					ComputeWorldTransform();
				}
			}
		}
		// スロープと足元線分が衝突した？
		if (otherCollider->GetColliderType() == ColliderTypeEnum::Slope)
		{
			if (ownCollider == mLineCollider)
			{
				info = ownCollider->GetCollisionInfo();
				mPosition = info.mCollisionPoint;
				mIsOnGround = true;
			}
		}
	}

	//プレイヤーとぶつかった？
	if (colliderTag == Tag::Player)
	{
		if (ownCollider == mHitBox)
		{
			info = ownCollider->GetCollisionInfo();
			mPosition += info.mFixVec;
			ComputeWorldTransform();
		}
	}

	// プレイヤーの攻撃とぶつかった?
	if (colliderTag == Tag::PlayerBullet)
	{
		if (ownCollider == mHitBox)
		{
			mIsHitTrig = true;
		}
	}

	//mIsHitTrig = false;
}
