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

// �Q�b�^�[ //
	float GetHealth();
private:
	// �{�b�N�X�R���C�_�[�̃|�C���^
	class BoxCollider* mBoxCollider;

	// ���C���R���C�_�[�̃|�C���^
	class LineCollider* mLineCollider;

	class Health* mHitPoint;
};