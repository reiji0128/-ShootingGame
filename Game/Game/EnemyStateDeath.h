#pragma once
#include "EnemyStateBase.h"

class EnemyStateDeath : public EnemyStateBase
{
public:
	EnemyStateDeath();
	~EnemyStateDeath();

	EnemyState Update(class EnemyActor* owner, float deltaTime)override;
	void Enter(class EnemyActor* owner, float deltaTime)override;

private:
	SkeletalMeshComponent* mSkelMeshComp;
};