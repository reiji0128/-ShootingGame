#include "FollowCamera.h"
#include "Actor.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="owner"></param>
FollowCamera::FollowCamera(Actor* owner)
    :CameraComponent(owner)
    ,mHorzDist(350.0f)
    , mVertDist(150.0f)
    , mTargetDist(100.0f)
    , mSpringConstant(64.0f)
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void FollowCamera::Update(float deltaTime)
{
    CameraComponent::Update(deltaTime);

    // ばね定数から減衰を計算
    float dampening = 2.0f * Math::Sqrt(mSpringConstant);

    // 理想の位置を計算
    Vector3 idealPos = ComputeCameraPos();

    // 実際と理想の差を計算する
    Vector3 diff = mActualPos - idealPos;
    // ばねによる加速度を計算
    Vector3 acel = -mSpringConstant * diff -
        dampening * mVelocity;

    // 速度の更新
    mVelocity += acel * deltaTime;
    // 実際のカメラポジションを更新
    mActualPos += mVelocity * deltaTime;

    // ターゲットは所有アクターから前方に離れた座標
    Vector3 target = mOwner->GetPosition() + 
                      mOwner->GetForward() * mTargetDist;

    Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

    SetViewMatrix(view);
}

void FollowCamera::SnapToIdle()
{
    // 実際の位置は理想の位置と同じ
    mActualPos = ComputeCameraPos();
    // 速度はゼロ
    mVelocity = Vector3::Zero;
    // 注視点とビューを計算
    Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
    Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

    SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const
{
    // カメラの位置を所有アクターの上後方にセット
    Vector3 cameraPos = mOwner->GetPosition();
    cameraPos -= mOwner->GetForward() * mHorzDist;
    cameraPos += Vector3::UnitZ * mVertDist;
    return cameraPos;
}
