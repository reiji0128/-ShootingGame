#include "PlayerActor.h"
#include "Game.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "Animation.h"
#include "LineCollider.h"
#include "Input.h"
#include "MoveComponent.h"

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
{
	// 座標のセット
	SetPosition(pos);

	// スケールのセット
	SetScale(scale);

	// メッシュの読み込み・セット
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this);
	mSkeltalMeshComp->SetMesh(mesh);

	// スケルトンの読み込み
	mSkeltalMeshComp->SetSkeleton(RENDERER->GetSkeleton(gpskelFileName));
	
	// 当たり判定のセット
	AABB box = mesh->GetCollisionBox();
	box.Scaling(1.0f);
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
}

/// <summary>
/// アクター固有の入力処理
/// </summary>
/// <param name="keys"></param>
void PlayerActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	// W・A・S・D移動
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400.0f;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);

	// マウス移動
	// SDLから相対運動を取得
	int x, y;
	Uint32 buttons =  SDL_GetRelativeMouseState(&x, &y);
	// マウスの動きは、通常-500から+500の範囲
	const int maxMouseSpeed = 500;
	// 最大移動量における角速度
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;

	if (x != 0)
	{
		// およそ[-1.0,1.0]の範囲に変換
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		// 最大移動量での角速度を掛ける
		angularSpeed *= maxAngularSpeed;
	}

	mMoveComp->SetAngularSpeed(angularSpeed);
}

/// <summary>
/// 衝突処理
/// </summary>
/// <param name="ownCollider"></param>
/// <param name="otherCollider"></param>
void PlayerActor::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}