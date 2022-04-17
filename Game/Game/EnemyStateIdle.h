#pragma once
#include "EnemyStateBase.h"
#include "SkeletalMeshComponent.h"

class EnemyStateIdle : public EnemyStateBase
{
public:
	EnemyStateIdle();
	~EnemyStateIdle();

	EnemyState Update(class EnemyActor* owner, float deltaTime) override;
	void Enter(class EnemyActor* owner, float deltaTime) override;

private:
	SkeletalMeshComponent* mSkelMeshComp;
};