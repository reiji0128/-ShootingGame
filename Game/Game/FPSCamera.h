#pragma once
#include "CameraComponent.h"

class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class Actor* owner);

	void Update(float deltaTime) override;

// �Z�b�^�[ //
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }

// �Q�b�^�[ //
	float GetPitch() const { return mPitch; }
	float GetPitchSpeed() const{ return mPitchSpeed;}
	float GetMaxPitch() const { return mMaxPitch; }

private:
	// �s�b�`�̊p���x
	float mPitchSpeed;

	// �ő�s�b�`�p�x
	float mMaxPitch;

	// ���݂̃s�b�`
	float mPitch;
};
