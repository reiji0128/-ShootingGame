#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "ColliderComponent.h"
#include <algorithm>
#include <typeinfo>

// 管理ID用の静的カウンタの初期化
int Actor::mGlobalActorNo = 0;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="actorTag">Actorのタグ</param>
Actor::Actor(Tag actorTag)
	: mTag(actorTag)
	, mState(EActive)
	, mPosition(Vector3::Zero)
	, mDirection(Vector3::UnitX)
	, mRotation(Quaternion::Identity)
	, mScale(1.0f)
	, mRecomputeWorldTransform(true)
	, mID(mGlobalActorNo)
	, mSpeed(50.0f)
{
	//ゲームシステム本体に アクターを追加
	GAMEINSTANCE.AddActor(this);
	mGlobalActorNo++;
}

/// <summary>
/// デストラクタ
/// </summary>
Actor::~Actor()
{
	// アクターが持っているコンポーネントの削除
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
	// ゲームシステム本体にこのアクターの削除を依頼
	GAMEINSTANCE.RemoveActor(this);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void Actor::Update(float deltaTime)
{
	// アクターが生きているときのみ処理
	if (mState == EActive)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeWorldTransform();
	}
}

/// <summary>
/// コンポーネントの更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}

	// 死んでいるコンポーネントを一時保管
	for (auto comp : mComponents)
	{
		if (comp->GetState() == Component::EDelete)
		{
			mDeleteComponents.emplace_back(comp);
		}
	}

	// 消去コンポーネントをdelete mComponentsからもDeleteされる
	for (auto comp : mDeleteComponents)
	{
		delete comp;
	}
	mDeleteComponents.clear();
}

/// <summary>
/// Actor専用の更新処理
/// </summary>
/// <param name="deltaTime"></param>
void Actor::UpdateActor(float deltaTime)
{
}

/// <summary>
/// 入力処理
/// </summary>
void Actor::ProcessInput()
{
	if (mState == EActive)
	{
		// 入力処理を受け取るコンポーネントを優先して実行
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{
}

/// <summary>
/// 衝突処理
/// </summary>
/// <param name="ownCollider">Actorの所有するColliderComponentのポインタ</param>
/// <param name="otherCollider">衝突相手のColliderComponentのポインタ</param>
void Actor::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}

/// <summary>
/// 前進ベクトルから回転を計算
/// </summary>
void Actor::RotateToNewForward()
{
	// X軸ベクトル(1,0,0)とmDirectionの間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, mDirection);
	float angle = Math::Acos(dot);
	// 下向きだった場合
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 上向きだった場合
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとmDirectionとの外積から回転軸をもとめ、回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, mDirection);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

/// <summary>
/// 前進ベクトルから回転を計算
/// </summary>
/// <param name="forward">前方ベクトル</param>
void Actor::RotateToNewForward(const Vector3& forward)
{
	// X軸ベクトル(1,0,0)とmDirectionの間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);
	// 下向きだった場合
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 上向きだった場合
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとmDirectionとの外積から回転軸をもとめ、回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

/// <summary>
/// ワールド変換行列の計算
/// </summary>
void Actor::ComputeWorldTransform()
{
	//ワールド変換の再計算が必要なら実行
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		//前進ベクトルから回転を計算
		RotateToNewForward();

		// スケーリング→回転→平行移動となるように変換行列を作成
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// アクターが持っている全コンポーネントの変換を促す
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

/// <summary>
/// コンポーネントの追加
/// </summary>
/// <param name="component">追加するComponentのポインタ</param>
void Actor::AddComponent(Component* component)
{
	// コンポーネントをソートして追加
	// 自分のオーダー番号よりも大きい挿入点を見つける
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	// 要素を見つけたポイントの手前に挿入する
	mComponents.insert(iter, component);
}

/// <summary>
/// コンポーネントの削除 
/// </summary>
/// <param name="component">削除するComponentのポインタ</param>
void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
