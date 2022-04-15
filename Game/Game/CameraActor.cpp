#include "CameraActor.h"
#include "Game.h"

CameraActor::CameraActor(Actor* targetActor)
	:Actor(Tag::Camera)
	,mTargetActor(targetActor)
{
	mPosition = Vector3(0, 100, 100);

	// デフォルトカメラ値セット
	mViewMatrix = Matrix4::CreateLookAt(Vector3(-100, 0, 100),
		                                Vector3(0, 0, 0),
		                                Vector3(0, 0, 1));

	// ゲームシステム側にアクティブを通知
	SetActive();
}

CameraActor::~CameraActor()
{
	// ゲームシステム側に非アクティブを通知
	GAMEINSTANCE.SetInActiveCameraActor(this);
}

void CameraActor::Update(float deltaTime)
{
	UpdateActor(deltaTime);

	// カメラ視線ベクトル,カメラ行列作成
	mViewVector = mViewTarget - mPosition;
	mViewMatrix = Matrix4::CreateLookAt(mPosition, 
		                                mViewTarget, 
		                                Vector3(0, 0, 1));
}

void CameraActor::SetActive()
{
	// ゲームシステム側にアクティブを通知
	GAMEINSTANCE.SetCameraActor(this);
}

void CameraActor::Init(const Vector3& cameraPos, const Vector3& targetPos, const Vector3& upVec)
{
	mPosition = cameraPos;
	mViewTarget = targetPos;
	mViewVector = targetPos - cameraPos;

	mViewMatrix = Matrix4::CreateLookAt(cameraPos,
		                                targetPos,
		                                upVec);
}