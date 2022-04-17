#pragma once
#include "EnemyActor.h"

const float nearLength = 250.0f;   //追跡中止距(攻撃に移行)
const float trackingRange = 500.0f; // 追跡距離
const float waitTime = 2.0f;       // 待機時間

class EnemyStateBase
{
public:
	EnemyStateBase()
		:mStateTimer(0.0f) {};

	virtual ~EnemyStateBase() {};

	virtual EnemyState Update(class EnemyActor* owner, float deltaTime) = 0;

	virtual void Enter(class EnemyActor* owner, float deltaTime) {};

protected:
	// ステート内の経過時刻を測るタイマー
	float mStateTimer;
};