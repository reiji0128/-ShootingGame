#include "EnemyStateIdle.h"
#include "Game.h"

EnemyStateIdle::EnemyStateIdle()
{
}

EnemyStateIdle::~EnemyStateIdle()
{
}

EnemyState EnemyStateIdle::Update(EnemyActor* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	// プレイヤー位置情報
	Vector3 playerPos = GAMEINSTANCE.GetPlayerActor()->GetPosition();
	Vector3 mPosition = owner->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - mPosition;

	float length = enemyToPlayerVec.Length();

	//追跡しない距離まで近づいたときに攻撃に移行
	if (length < nearLength)
	{
		return EnemyState::STATE_ATTACK;
	}

	if (mStateTimer < 0.0f)
	{
		return EnemyState::STATE_PATROL;
	}

	if (owner->GetIsHitTrig())
	{
		return EnemyState::STATE_DEATH;
	}

	return EnemyState::STATE_IDLE;
}

void EnemyStateIdle::Enter(EnemyActor* owner, float deltaTime)
{
	//アイドル状態のアニメーション再生
	mSkelMeshComp = owner->GetSkeltalMeshComp();
	mSkelMeshComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_IDLE));

	mStateTimer = waitTime;
	owner->SetVelosity(Vector3::Zero);
}
