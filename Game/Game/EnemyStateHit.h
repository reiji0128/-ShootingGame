#pragma once
#include "EnemyStateBase.h"

class EnemyStateHit : public EnemyStateBase
{
public:
	EnemyStateHit();

	~EnemyStateHit();

	EnemyState Update(class EnemyActor* owner, float delotaTime)override;

	void Enter(class EnemyActor* owner, float deltaTime)override;

	const float knokBackSpeed = 500.0f;     // ダメージノックバック速度
	const float knokBackBrake = 0.92f;      // ダメージノックバック減速率

	SkeletalMeshComponent* mSkelMeshComp;
};