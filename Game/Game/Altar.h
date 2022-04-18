#pragma once
#include "Actor.h"
#include "Math.h"

class Altar : public Actor
{
public:
	Altar(const Vector3& pos,const char* gpmeshFileName);
	~Altar();
	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(class ColliderComponent* ownCollider, class ColliderComponent* otherCollider)override;

// ゲッター //
	float GetHealth();
private:
	// ボックスコライダーのポインタ
	class BoxCollider* mBoxCollider;

	// ラインコライダーのポインタ
	class LineCollider* mLineCollider;

	class Health* mHitPoint;
};