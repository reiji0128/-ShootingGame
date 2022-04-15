#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">���̃N���X�����L����Actor</param>
	FollowCamera(class Actor* owner);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void Update(float deltaTime) override;

	void SnapToIdle();

	void SetHorzDist(float dist) { mHorzDist = dist; }

	void SetVertDist(float dist) { mVertDist = dist; }

	void SetTargetDist(float dist) { mTargetDist = dist; }

	void SetSpringConstant(float spring) { mSpringConstant = spring; }

private:
	Vector3 ComputeCameraPos() const;

	// �J�����̈ʒu
	Vector3 mActualPos;

	// �J�����̑��x
	Vector3 mVelocity;

	// �����Ǐ]����
	float mHorzDist;

	// �����Ǐ]����
	float mVertDist;

	// �ڕW����
	float mTargetDist;

	// �΂˒萔�i�����قǍd���j
	float mSpringConstant;
};