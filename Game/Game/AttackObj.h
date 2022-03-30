#pragma once
#include "Actor.h"
#include "Collision.h"

class AttackObj : public Actor
{
public:
	AttackObj(Tag attackerType, const Vector3& pos, const Vector3& dir, float scale, float lifeTime = 1.0f);
	AttackObj(Tag attackerType, const Vector3& pos, const Vector3& dir, const AABB& hitRange, float lifeTime = 1.0f);

	void UpdateActor(float deltaTime)override;

private:
	float mLifeTime  ; // 寿命
	float mAliveTime ; // 現在まで生きた時間

};