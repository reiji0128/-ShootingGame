#include "FollowCamera.h"
#include "Actor.h"

/// <summary>
/// �R���X�g���N�^
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
/// �X�V����
/// </summary>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void FollowCamera::Update(float deltaTime)
{
    CameraComponent::Update(deltaTime);

    // �΂˒萔���猸�����v�Z
    float dampening = 2.0f * Math::Sqrt(mSpringConstant);

    // ���z�̈ʒu���v�Z
    Vector3 idealPos = ComputeCameraPos();

    // ���ۂƗ��z�̍����v�Z����
    Vector3 diff = mActualPos - idealPos;
    // �΂˂ɂ������x���v�Z
    Vector3 acel = -mSpringConstant * diff -
        dampening * mVelocity;

    // ���x�̍X�V
    mVelocity += acel * deltaTime;
    // ���ۂ̃J�����|�W�V�������X�V
    mActualPos += mVelocity * deltaTime;

    // �^�[�Q�b�g�͏��L�A�N�^�[����O���ɗ��ꂽ���W
    Vector3 target = mOwner->GetPosition() + 
                      mOwner->GetForward() * mTargetDist;

    Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

    SetViewMatrix(view);
}

void FollowCamera::SnapToIdle()
{
    // ���ۂ̈ʒu�͗��z�̈ʒu�Ɠ���
    mActualPos = ComputeCameraPos();
    // ���x�̓[��
    mVelocity = Vector3::Zero;
    // �����_�ƃr���[���v�Z
    Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
    Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

    SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const
{
    // �J�����̈ʒu�����L�A�N�^�[�̏����ɃZ�b�g
    Vector3 cameraPos = mOwner->GetPosition();
    cameraPos -= mOwner->GetForward() * mHorzDist;
    cameraPos += Vector3::UnitZ * mVertDist;
    return cameraPos;
}
