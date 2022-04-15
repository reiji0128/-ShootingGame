#pragma once
#include "Actor.h"

class CameraActor : public Actor
{
public:
	CameraActor(Actor* targetActor);
	virtual ~CameraActor();
	
	void Update(float deltaTime);
	void SetActive();
	void Init(const Vector3& cameraPos, const Vector3& targetPos,
		      const Vector3& upVec);
	Matrix4& GetViewMatrix() { return mViewMatrix; }
	Vector3& GetViewVector() { return mViewTarget; }
	Vector3& GetViewTarget() { return mViewTarget; }
	Vector3& GetViewPos() { return mPosition; }
protected:
	// カメラが基準とするアクター
	Actor* mTargetActor;

	// ビュー行列
	Matrix4 mViewMatrix;

	// カメラ注視点
	Vector3 mViewTarget;

	// 視線ベクトル
	Vector3 mViewVector;
};