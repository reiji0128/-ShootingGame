#pragma once

#include <cstdint>

class Component
{
public:
	/// <summary>
	/// コンポーネントの状態
	/// </summary>
	enum ComponentState
	{
		EActive,
		EPaused,
		EDelete
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">Actorのポインタ</param>
	/// <param name="updateOrder">更新順序(更新順序が低いほど、コンポーネントの更新が早い)</param>
	Component(class Actor* owner, int updateOrder = 100);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Component();

	/// <summary>
	/// Componentの更新処理
	/// </summary>
	/// <param name="deltaTiem">1フレームの経過時間</param>
	virtual void Update(float deltaTiem);

	/// <summary>
	/// Componentの入力処理
	/// </summary>
	virtual void ProcessInput(){}

	/// <summary>
	/// ワールド変換処理
	/// </summary>
	virtual void OnUpdateWorldTransform(){}

// セッター //
	/// <summary>
	/// Componentの状態のセット
	/// </summary>
	void SetState(ComponentState newState) { mState = newState; }

// ゲッター //
	/// <summary>
	/// Componentの管理IDの取得
	/// </summary>
	/// <returns>Componentの管理ID</returns>
	int GetID() { return mID; }

	/// <summary>
	/// オーナーのポインタの取得
	/// </summary>
	/// <returns>オーナーのポインタ</returns>
	class Actor* GetOwner() { return mOwner; }

	/// <summary>
	/// 更新順序の取得
	/// </summary>
	/// <returns>更新順序</returns>
	int GetUpdateOrder() const { return mUpdateOrder; }

	/// <summary>
	/// コンポーネントの状態の取得
	/// </summary>
	/// <returns>コンポーネントの状態</returns>
	ComponentState GetState() { return mState; }
protected:
	// 更新順序
	int mUpdateOrder;

	// コンポーネントの管理ID
	int mID;

	// コンポーネントの管理IDの連番用
	static int mGlobalID;

	//所有しているActorのポインタ
	class Actor* mOwner;

	//コンポーネントの状態
	ComponentState mState;
};