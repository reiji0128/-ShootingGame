#pragma once
#include "EnemyStateBase.h"

class EnemyStateRun : public EnemyStateBase
{
public:
	EnemyStateRun();

	~EnemyStateRun();

	EnemyState Update(class EnemyActor* owner, float deltaTime) override;

	void Enter(class EnemyActor* owner, float deltaTime) override;

private:

	void MoveCalc(EnemyActor* owner, float deltaTime);

	float length;
	const float runSpeed = 150.0f;   //í«ê’ë¨ìx

	Vector3 enemyToPlayerVec;

	SkeletalMeshComponent* mSkelMeshComp;
};