#pragma once

#include <vector>
#include <cstdint>
#include "Math.h"
#include "Tag.h"

class Actor
{
public:
	/// <summary>
	/// Actorの状態
	/// </summary>
	enum State
	{
		EActive,
		EInactive,
		EPaused,
		EDead
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="actorTag">Actorのタグ</param>
	Actor(Tag actorTag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Actor();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void              Update(float deltaTime);

	/// <summary>
	/// コンポーネントの更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void              UpdateComponents(float deltaTime);

	/// <summary>
	/// Actor専用の更新処理
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void      UpdateActor(float deltaTime);

	/// <summary>
	/// 入力処理
	/// </summary>
	void              ProcessInput();


	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="ownCollider">Actorの所有するColliderComponentのポインタ</param>
	/// <param name="otherCollider">衝突相手のColliderComponentのポインタ</param>
	virtual void      OnCollisionEnter(class ColliderComponent* ownCollider, class ColliderComponent* otherCollider);                         // 他の物体と当たった時

	// Getters/setters　ゲッター・セッター

// セッター //
	
	/// <summary>
	/// 位置のセット
	/// </summary>
	/// <param name="pos">セットする座標</param>
	void              SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	
	/// <summary>
	/// スケールのセット(float用)
	/// </summary>
	/// <param name="scale">スケール倍率</param>
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	
	/// <summary>
	/// 回転(クォータニオン)のセット
	/// </summary>
	/// <param name="rotation">セットする回転角</param>
	void              SetRotation(const Quaternion& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }  // 回転（クォータニオン）のセット
	
	/// <summary>
	/// 向きのセット
	/// </summary>
	/// <param name="dir">セットする方向</param>
	void              SetDirection(const Vector3& dir) { mDirection = dir; }
	
	/// <summary>
	/// ワールド変換処理をするか？ (移動など変換行列の再計算が必要な時)
	/// </summary>
	void              SetComputeWorldTransform() { mRecomputeWorldTransform = true; }
	
	/// <summary>
	/// Actorのスピードのセット
	/// </summary>
	/// <param name="speed">セットするスピード</param>
	void              SetSpeed(float speed) { mSpeed = speed; }
	
	/// <summary>
	/// Actorの状態のセット
	/// </summary>
	/// <param name="state">セットする状態</param>
	void              SetState(State state) { mState = state; }                                    // アクターの状態セット
	
// ゲッター //
	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns>Actorの位置</returns>
	const Vector3&    GetPosition() const { return mPosition; }

	/// <summary>
	/// スケールの取得
	/// </summary>
	/// <returns>Actorのスケール</returns>
	const float&    GetScale() const { return mScale; }

	/// <summary>
	/// 回転(クォータニオン)の取得
	/// </summary>
	/// <returns>回転角</returns>
	const Quaternion& GetRotation() const { return mRotation; }

	/// <summary>
	/// 向きの取得
	/// </summary>
	/// <returns>Actorの向き</returns>
	const Vector3&    GetDirection() const { return mDirection; }

	/// <summary>
	/// タグの取得
	/// </summary>
	/// <returns>タグの種類</returns>
	Tag               GetTag() { return mTag; }

	/// <summary>
	/// ワールド変換行列の取得
	/// </summary>
	/// <returns>ワールド変換行列</returns>
	const Matrix4&    GetWorldTransform() const { return mWorldTransform; }

	/// <summary>
	/// 前方ベクトルの取得
	/// </summary>
	/// <returns>前方ベクトル</returns>
	Vector3           GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }
	
	/// <summary>
	/// 右方向ベクトルの取得
	/// </summary>
	/// <returns>右方向ベクトル</returns>
	Vector3           GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }
	
	/// <summary>
	/// 状態の取得
	/// </summary>
	/// <returns>Actorの状態</returns>
	State             GetState() const { return mState; }
	
	/// <summary>
	/// Actorのスピードの取得
	/// </summary>
	/// <returns>Actorのスピード</returns>
	float             GetSpeed() const { return mSpeed; }
	
	/// <summary>
	/// Actorの管理IDの取得
	/// </summary>
	/// <returns>Actorの管理ID</returns>
	int               GetID() { return mID; };

	/// <summary>
	/// 前進ベクトルから回転を計算
	/// </summary>
	void              RotateToNewForward();

	/// <summary>
	/// 前進ベクトルから回転を計算
	/// </summary>
	/// <param name="forward">前方ベクトル</param>
	void              RotateToNewForward(const Vector3& forward);

	/// <summary>
	/// ワールド変換行列の計算
	/// </summary>
	void              ComputeWorldTransform();

	/// <summary>
	/// コンポーネントの追加
	/// </summary>
	/// <param name="component">追加するComponentのポインタ</param>
	void              AddComponent(class Component* component);

	/// <summary>
	/// コンポーネントの削除 
	/// </summary>
	/// <param name="component">削除するComponentのポインタ</param>
	void              RemoveComponent(class Component* component);


protected:
	// Actorの位置
	Vector3           mPosition;

	// Actorの向き
	Vector3           mDirection;

	// Actorの回転
	Quaternion        mRotation;

	// スケール
	float             mScale;

	// スピード
	float             mSpeed;

	// Actorのタグ
	Tag               mTag;

	// Actorの状態
	State             mState;

	// ワールド変換行列
	Matrix4           mWorldTransform;


	// ワールド変換の再計算が必要か？
	bool              mRecomputeWorldTransform;

	// 管理ID
	int               mID;

	// 管理ID用の静的カウンタ
	static int        mGlobalActorNo;

	// 所有するコンポーネントを格納するVector
	std::vector<class Component*> mComponents;

	// 削除するコンポーネントを格納するVector
	std::vector<class Component*> mDeleteComponents;

};