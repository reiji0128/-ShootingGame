#include "PlayerActor.h"
#include "Game.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "Animation.h"
#include "LineCollider.h"
#include "Input.h"
#include "ThirdPersonCameraActor.h"

#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pos">位置</param>
/// <param name="scale">スケール</param>
/// <param name="gpmeshFileName">gpmeshのパス</param>
/// <param name="gpskelFileName">gpskelのパス</param>
PlayerActor::PlayerActor(const Vector3& pos, const float& scale, const char* gpmeshFileName, const char* gpskelFileName)
	: Actor(Tag::Player)
	, mSkeltalMeshComp(nullptr)
	, mBoxCollider(nullptr)
	, mLineCollider(nullptr)
	, mBoneIndex(0)
	, mNowState()
	, mNextState()
{
	// 座標のセット
	SetPosition(pos);

	// スケールのセット
	SetScale(scale);

	SetDirection(Vector3(-1, 0, 0));

	// メッシュの読み込み・セット
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this);
	mSkeltalMeshComp->SetMesh(mesh);

	// スケルトンの読み込み
	mSkeltalMeshComp->SetSkeleton(RENDERER->GetSkeleton(gpskelFileName));
	
	// アニメーションの読み込み
	mAnimations.resize(static_cast<unsigned int>(PlayerState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)] = RENDERER->GetAnimation("Assets/Player/Rifle_Idle.gpanim", true);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_RUN)]  = RENDERER->GetAnimation("Assets/Player/Walk_Forward.gpanim", true);
	
	// アイドル状態のアニメーションをセット
	mSkeltalMeshComp->PlayAnimation(mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)]);
	
	// アクターステートプールの初期化
	mStatePools.push_back(new PlayerStateIdle);
	mStatePools.push_back(new PlayerStateRun);

	// 当たり判定のセット
	AABB box = mesh->GetCollisionBox();
	box.Scaling(1.0f,1.0f,1.0f);
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);

	// ラインコライダーのセット
	mLineCollider = new LineCollider(this);
	Line line(Vector3(0, 0, 50.0f), Vector3(0, 0, -20.0f));
	mLineCollider->SetLine(line);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerActor::~PlayerActor()
{
}

/// <summary>
/// プレイヤーの更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void PlayerActor::UpdateActor(float deltaTime)
{
	// ステート外部からステート変更があったか
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// ステート実行
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);
	
	// ステート内部から変更があったか
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}
}

/// <summary>
/// 衝突処理
/// </summary>
/// <param name="ownCollider"></param>
/// <param name="otherCollider"></param>
void PlayerActor::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
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
	}

	// エネミーとぶつかったか
	if (colliderTag == Tag::Enemy)
	{
		if (ownCollider == mHitBox)
		{
			info = ownCollider->GetCollisionInfo();
			mPosition += info.mFixVec;
			ComputeWorldTransform();
		}
	}

	if (colliderTag == Tag::Altar)
	{
		if (ownCollider == mHitBox)
		{
			info = ownCollider->GetCollisionInfo();
			mPosition += info.mFixVec;
			ComputeWorldTransform();
		}
	}

	//// プレイヤーの攻撃とぶつかった?
	//if (colliderTag == Tag::EnemyBullet)
	//{
	//	if (ownCollider == mHitBox)
	//	{
	//		mIsHitTrig = true;
	//	}
	//}
}
