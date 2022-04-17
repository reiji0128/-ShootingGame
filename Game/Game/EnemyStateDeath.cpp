#include "EnemyStateDeath.h"

EnemyStateDeath::EnemyStateDeath()
{
}

EnemyStateDeath::~EnemyStateDeath()
{
}

EnemyState EnemyStateDeath::Update(EnemyActor* owner, float deltaTime)
{
	if (!mSkelMeshComp->IsPlaying())
	{
		owner->SetState(owner->EDead);
	}

	return EnemyState::STATE_DEATH;
}

void EnemyStateDeath::Enter(EnemyActor* owner, float deltaTime)
{
	owner->SetVelosity(Vector3::Zero);

	//死亡状態のアニメーション再生
	mSkelMeshComp = owner->GetSkeltalMeshComp();
	mSkelMeshComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_DEATH));
}
