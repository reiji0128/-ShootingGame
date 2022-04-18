#pragma once
#include "Actor.h"

class BulletActor : public Actor
{
public:
	BulletActor(const Vector3& pos, const Vector3& dir, Tag type, float speed, float range);
	~BulletActor();
	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider) override;
	void SetShootRange(float length) { mShootRange = length; }
private:
	Vector3 mStartPos;
	float mShootRange;
	class BoxCollider* mHitBox;
};