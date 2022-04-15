#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">このクラスを所有するActor</param>
	FollowCamera(class Actor* owner);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void Update(float deltaTime) override;

	void SnapToIdle();

	void SetHorzDist(float dist) { mHorzDist = dist; }

	void SetVertDist(float dist) { mVertDist = dist; }

	void SetTargetDist(float dist) { mTargetDist = dist; }

	void SetSpringConstant(float spring) { mSpringConstant = spring; }

private:
	Vector3 ComputeCameraPos() const;

	// カメラの位置
	Vector3 mActualPos;

	// カメラの速度
	Vector3 mVelocity;

	// 水平追従距離
	float mHorzDist;

	// 垂直追従距離
	float mVertDist;

	// 目標距離
	float mTargetDist;

	// ばね定数（高いほど硬い）
	float mSpringConstant;
};