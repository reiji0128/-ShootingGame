#pragma once
#include "EnemyStateBase.h"

class EnemyStateAttack : public EnemyStateBase
{
public:
	EnemyStateAttack();
	~EnemyStateAttack();

	EnemyState Update(class EnemyActor* owner, float deltaTime)override;
	void Enter(class EnemyActor* owner, float deltaTime)override;

private:
	void Attack(class EnemyActor* owner, float deltaTime);

	int mRandomNum;

	const float retrackingRange = 250.0f;      // UŒ‚’†‚©‚ç’ÇÕ‚É•Ï‚í‚é‹——£
	const float attackOffsetTime = 0.5f;       // UŒ‚ƒAƒjƒŠJn‚µ‚Ä‚©‚çUŒ‚”»’è‚ª”­¶‚·‚é‚Ü‚Å‚Ì’²®ŠÔ

	SkeletalMeshComponent* mSkelMeshComp;
};