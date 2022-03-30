#include "AttackObj.h"
#include "Game.h"
#include "Math.h"
#include "Renderer.h"
#include "EffectComponent.h"
#include "BoxCollider.h"

AttackObj::AttackObj(Tag attackerType,const Vector3& pos, const Vector3& dir, float scale, float lifeTime)
	: Actor(attackerType)
	, mLifeTime(lifeTime)
	, mAliveTime(0.0f)
{
	mPosition = pos + dir * scale;

	AABB box;
	box.mMax = Vector3( scale * 0.5,  scale * 0.5, scale) ;
	box.mMin = Vector3(-scale * 0.5, -scale * 0.5, 0.0f ) ;
	box.mIsRotatable = false;

	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);

}

AttackObj::AttackObj(Tag attackerType, const Vector3& pos, const Vector3& dir, const AABB& hitRange, float lifeTime)
	: Actor(attackerType)
	, mLifeTime(lifeTime)
	, mAliveTime(0.0f)
{
	mPosition = pos;

	AABB box;
	box.mMax = hitRange.mMax;
	box.mMax = hitRange.mMin;
	box.mIsRotatable = false;

	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);

}

void AttackObj::UpdateActor(float deltaTime)
{
	mAliveTime += deltaTime;

	// 寿命が来たら当たり判定消滅
	if (mAliveTime > mLifeTime)
	{
		mState = Actor::EDead;
	}
}
