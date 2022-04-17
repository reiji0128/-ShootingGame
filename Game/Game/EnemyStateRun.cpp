#include "EnemyStateRun.h"
#include "Game.h"

EnemyStateRun::EnemyStateRun()
{
}

EnemyStateRun::~EnemyStateRun()
{
}

EnemyState EnemyStateRun::Update(EnemyActor* owner, float deltaTime)
{
	// プレイヤー位置情報
	Vector3 playerPos = GAMEINSTANCE.GetPlayerActor()->GetPosition();
	Vector3 pos = owner->GetPosition();
	enemyToPlayerVec = playerPos - pos;

	float length = enemyToPlayerVec.Length();
	enemyToPlayerVec.Normalize();

	//追跡しない距離まで近づいたときに攻撃に移行
	if (length < nearLength)
	{
		return EnemyState::STATE_ATTACK;
	}

	// 追跡可能距離を超えたらステートをパトロールに移行
	if (length > trackingRange)
	{
		return EnemyState::STATE_PATROL;
	}

	//プレイヤーの攻撃を受けたか
	if (owner->GetIsHitTrig())
	{
		return EnemyState::STATE_DEATH;
	}

	//移動処理
	MoveCalc(owner, deltaTime);
	return EnemyState::STATE_RUN;
}

void EnemyStateRun::Enter(EnemyActor* owner, float deltaTime)
{
	//Run状態のアニメーション再生
	mSkelMeshComp = owner->GetSkeltalMeshComp();
	mSkelMeshComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_RUN));
}

void EnemyStateRun::MoveCalc(EnemyActor* owner, float deltaTime)
{
	//水平方向の向きを取得
	Vector3 dir = enemyToPlayerVec;
	dir.z = 0.0f;
	dir.Normalize();

	Vector3 velocity = dir * runSpeed * deltaTime;
	owner->SetVelosity(velocity);

	owner->SetDirection(dir);
	owner->SetComputeWorldTransform();
}
