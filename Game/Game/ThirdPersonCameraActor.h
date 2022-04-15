#pragma once
#include "CameraActor.h"

class ThirdPersonCameraActor : public CameraActor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ThirdPersonCameraActor(Actor* targetActor);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ThirdPersonCameraActor();

	/// <summary>
	/// アクターの更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void UpdateActor(float deltaTime) override;

// セッター //
	void SetRotateZAngle(float angle) { mRotateZAngle = angle; }
	void SetLookDownAngle(float angle) { mLookDownAngle = angle; }
	void SetCameraLength(float length) { mCameraLength = length; }
	void SetCameraPos(Vector3& vec) { mCameraPos = vec; }

// ゲッター //
	Vector3& GetCameraForwardVec() { return mCameraForwardVec; }
	Vector3& GetCameraPos() { return mCameraPos; }

private:
	// プレイヤーからの距離
	float mCameraLength;

	// カメラ周囲回転方向回転角
	float mRotateZAngle;

	// カメラ見下ろし方向回転角
	float mLookDownAngle;

	// 前方ベクトル
	Vector3 mCameraForwardVec;

	// カメラ位置
	Vector3 mCameraPos;
};