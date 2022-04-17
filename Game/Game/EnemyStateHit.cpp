#include "EnemyStateHit.h"
#include "Game.h"

EnemyStateHit::EnemyStateHit()
{
}

EnemyStateHit::~EnemyStateHit()
{
}

EnemyState EnemyStateHit::Update(EnemyActor* owner, float delotaTime)
{
	// 減速
	owner->SetVelosity(owner->GetVelosity() * knokBackBrake);

	if (!mSkelMeshComp->IsPlaying())
	{
		owner->SetVelosity(Vector3::Zero);
		owner->SetIsHitTrig(false);
		//死亡処理
		/*if (health->GetPre() <= health->GetMin())
		{
			return EnemyState::STATE_DEATH;
		}*/

		return EnemyState::STATE_PATROL;
	}

	return EnemyState::STATE_HIT;
}

void EnemyStateHit::Enter(EnemyActor* owner, float deltaTime)
{
	owner->SetVelosity(Vector3::Zero);

	//ヒット状態のアニメーション再生
	mSkelMeshComp = owner->GetSkeltalMeshComp();
	mSkelMeshComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_HIT));

	// プレイヤーの位置からノックバック
	Vector3 knokBackDir = owner->GetPosition() - GAMEINSTANCE.GetPlayerActor()->GetPosition();
	knokBackDir.Normalize();

	//ノックバックの初速ベクトル
	owner->SetVelosity(knokBackDir * knokBackSpeed * deltaTime);

	//ダメージ処理
	/*Health* health = owner->GetEnemyHealth();
	health->SetPre(health->GetPre() - 5.0f);*/
}
