#include "PlayerActor.h"
#include "Game.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "Animation.h"
#include "LineCollider.h"
#include "Input.h"
#include "ThirdPersonCameraActor.h"

#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pos">�ʒu</param>
/// <param name="scale">�X�P�[��</param>
/// <param name="gpmeshFileName">gpmesh�̃p�X</param>
/// <param name="gpskelFileName">gpskel�̃p�X</param>
PlayerActor::PlayerActor(const Vector3& pos, const float& scale, const char* gpmeshFileName, const char* gpskelFileName)
	: Actor(Tag::Player)
	, mSkeltalMeshComp(nullptr)
	, mBoxCollider(nullptr)
	, mLineCollider(nullptr)
	, mBoneIndex(0)
	, mNowState()
	, mNextState()
{
	// ���W�̃Z�b�g
	SetPosition(pos);

	// �X�P�[���̃Z�b�g
	SetScale(scale);

	SetDirection(Vector3(-1, 0, 0));

	// ���b�V���̓ǂݍ��݁E�Z�b�g
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this);
	mSkeltalMeshComp->SetMesh(mesh);

	// �X�P���g���̓ǂݍ���
	mSkeltalMeshComp->SetSkeleton(RENDERER->GetSkeleton(gpskelFileName));
	
	// �A�j���[�V�����̓ǂݍ���
	mAnimations.resize(static_cast<unsigned int>(PlayerState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)] = RENDERER->GetAnimation("Assets/Player/Rifle_Idle.gpanim", true);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_RUN)]  = RENDERER->GetAnimation("Assets/Player/Walk_Forward.gpanim", true);
	
	// �A�C�h����Ԃ̃A�j���[�V�������Z�b�g
	mSkeltalMeshComp->PlayAnimation(mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)]);
	
	// �A�N�^�[�X�e�[�g�v�[���̏�����
	mStatePools.push_back(new PlayerStateIdle);
	mStatePools.push_back(new PlayerStateRun);

	// �����蔻��̃Z�b�g
	AABB box = mesh->GetCollisionBox();
	box.Scaling(1.0f);
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);

	// ���C���R���C�_�[�̃Z�b�g
	mLineCollider = new LineCollider(this);
	Line line(Vector3(0, 0, 50.0f), Vector3(0, 0, -20.0f));
	mLineCollider->SetLine(line);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerActor::~PlayerActor()
{
}

/// <summary>
/// �v���C���[�̍X�V����
/// </summary>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void PlayerActor::UpdateActor(float deltaTime)
{
	// �X�e�[�g�O������X�e�[�g�ύX����������
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// �X�e�[�g���s
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);
	
	// �X�e�[�g��������ύX����������
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}
}

/// <summary>
/// �Փˏ���
/// </summary>
/// <param name="ownCollider"></param>
/// <param name="otherCollider"></param>
void PlayerActor::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}
