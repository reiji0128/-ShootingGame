#include "EnemyStateAttack.h"
#include "Game.h"
#include "AttackObj.h"

EnemyStateAttack::EnemyStateAttack()
{
}

EnemyStateAttack::~EnemyStateAttack()
{
}

EnemyState EnemyStateAttack::Update(EnemyActor* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	// プレイヤーから離れたら追跡モードへ
	Vector3 playerPos = GAMEINSTANCE.GetFirstActor(Tag::Altar)->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();

	// プレイヤーが離れたら追跡モードへ移行
	if (length > retrackingRange)
	{
		return EnemyState::STATE_RUN;
	}

	// 再生アニメが完了したら追跡モードへ移行
	if (!mSkelMeshComp->IsPlaying())
	{
		return EnemyState::STATE_RUN;
	}

	//プレイヤーの攻撃を受けたか
	if (owner->GetIsHitTrig())
	{
		return EnemyState::STATE_DEATH;
	}

	//攻撃処理
	Attack(owner, deltaTime);

	return EnemyState::STATE_ATTACK;
}

void EnemyStateAttack::Enter(EnemyActor* owner, float deltaTime)
{
	mStateTimer = attackOffsetTime;
	owner->SetVelosity(Vector3::Zero);

	//攻撃状態のアニメーション再生
	mSkelMeshComp = owner->GetSkeltalMeshComp();
	mSkelMeshComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_ATTACK));
}

void EnemyStateAttack::Attack(EnemyActor* owner, float deltaTime)
{
	//攻撃可能時間になったら攻撃
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::EnemyBullet, owner->GetPosition(), owner->GetDirection(), 100.0f, 0.3f);
		mStateTimer = FLT_MAX;		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
	}
}
