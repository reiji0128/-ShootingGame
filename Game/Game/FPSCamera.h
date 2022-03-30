#pragma once
#include "CameraComponent.h"

class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class Actor* owner);

	void Update(float deltaTime) override;

// セッター //
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }

// ゲッター //
	float GetPitch() const { return mPitch; }
	float GetPitchSpeed() const{ return mPitchSpeed;}
	float GetMaxPitch() const { return mMaxPitch; }

private:
	// ピッチの角速度
	float mPitchSpeed;

	// 最大ピッチ角度
	float mMaxPitch;

	// 現在のピッチ
	float mPitch;
};
