#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime)override;

// ゲッター //
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	float GetStrafeSpeed() const { return mStrafeSpeed; }

// セッター //
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }
private:
	// 回転スピード
	float mAngularSpeed;

	// 前進スピード
	float mForwardSpeed;

	// ストレイフスピード
	float mStrafeSpeed;
};