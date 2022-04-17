#pragma once
#include "EnemyStateBase.h"

class EnemyStatePatrol : public EnemyStateBase
{
public:
	EnemyStatePatrol();

	~EnemyStatePatrol();

	EnemyState Update(class EnemyActor* owner, float deltaTime) override;

	void Enter(class EnemyActor* owner, float deltaTime) override;

private:
	const float patrolSpeed = 100.0f;   //パトロール時のスピード
	const float rotateSpeed = 45.0f;    // 回転スピード

	Vector3 enemyToPlayerVec;			// エネミーからプレイヤーに向かうベクトル
	Vector3 mAimVec;                    // 目標回転方向
	bool mRotateNow;                    // 回転中か？

	SkeletalMeshComponent* mSkelMeshComp;
};