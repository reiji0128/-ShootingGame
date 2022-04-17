#include "EnemyStatePatrol.h"
#include "Game.h"

EnemyStatePatrol::EnemyStatePatrol()
	:mAimVec(0, 0, 0)
	, mRotateNow(false)
{
}

EnemyStatePatrol::~EnemyStatePatrol()
{
}

EnemyState EnemyStatePatrol::Update(EnemyActor* owner, float deltaTime)
{
	Vector3 dir = owner->GetDirection();

	//‰ñ“]’†‚©?
	if (mRotateNow)
	{
		// –Ú•WŠp“x‚É‹ß‚Ã‚¢‚½‚ç‰ñˆÚ“]I—¹E’¼i‚Ö
		if (isNearAngle(dir, mAimVec))
		{
			dir = mAimVec;
			mRotateNow = false;
			owner->SetDirection(dir);
			owner->SetVelosity(dir * patrolSpeed * deltaTime);
			return EnemyState::STATE_PATROL;
		}
		// ‰ñ“]
		dir = (zRotateToAimVec(dir, mAimVec, rotateSpeed * deltaTime));
		owner->SetDirection(dir);
		owner->RotateToNewForward();
	}

	// “GƒLƒƒƒ‰‚ÌŽ‹ŠE&”ÍˆÍ‚É“ü‚Á‚½‚ç’Ç”öƒ‚[ƒh‚Ö
	Vector3 playerPos = GAMEINSTANCE.GetPlayerActor()->GetPosition();
	enemyToPlayerVec = playerPos - owner->GetPosition();
	float dot = Vector3::Dot(enemyToPlayerVec, dir);

	if (dot > 0 && enemyToPlayerVec.Length() < trackingRange)
	{
		return EnemyState::STATE_RUN;
	}

	//ƒvƒŒƒCƒ„[‚ÌUŒ‚‚ðŽó‚¯‚½‚©
	if (owner->GetIsHitTrig())
	{
		return EnemyState::STATE_DEATH;
	}

	// BGTrigger‚ª”wŒi‚ÉG‚ê‚½‚ç‰ñ“]ƒ‚[ƒh‚É•ÏX
	if (owner->GetIsBGTrig())
	{
		// 90“xŽžŒv‰ñ‚è‚É•ûŒü“]Š·&‰ñ“]ƒ‚[ƒh‚ÉˆÚs
		mRotateNow = true;
		mAimVec = Vector3::Cross(Vector3::UnitZ, dir);
		owner->SetVelosity(Vector3::Zero);
	}

	owner->SetDirection(dir);

	// ‘Oi
	owner->SetVelosity(dir * patrolSpeed * deltaTime);

	return EnemyState::STATE_PATROL;
}

void EnemyStatePatrol::Enter(EnemyActor* owner, float deltaTime)
{
	//ƒpƒgƒ[ƒ‹ó‘Ô‚ÌƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
	mSkelMeshComp = owner->GetSkeltalMeshComp();
	mSkelMeshComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_PATROL));

	mRotateNow = false;
}