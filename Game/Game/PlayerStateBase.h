#pragma once
#include "PlayerActor.h"

class PlayerStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerStateBase() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateBase() {};

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="owner">プレイヤーのポインタ</param>
	/// <param name="deltaTime">1フレームの経過時間</param>
	/// <returns>プレイヤーの状態</returns>
	virtual PlayerState Update(class PlayerActor* owner, float deltaTime) = 0;

	/// <summary>
	/// 状態が移行したときに1度だけする処理
	/// </summary>
	/// <param name="owner">プレイヤーのポインタ</param>
	/// <param name="deltaTime">1フレームの経過時間</param>
	virtual void Enter(class PlayerActor* owner, float deltaTime) {};

protected:

};