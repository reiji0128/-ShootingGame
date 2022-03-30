#include "PlayerActor.h"
#include "Game.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "Animation.h"
#include "LineCollider.h"
#include "Input.h"
#include "MoveComponent.h"

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
{
	// ���W�̃Z�b�g
	SetPosition(pos);

	// �X�P�[���̃Z�b�g
	SetScale(scale);

	// ���b�V���̓ǂݍ��݁E�Z�b�g
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this);
	mSkeltalMeshComp->SetMesh(mesh);

	// �X�P���g���̓ǂݍ���
	mSkeltalMeshComp->SetSkeleton(RENDERER->GetSkeleton(gpskelFileName));
	
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
}

/// <summary>
/// �A�N�^�[�ŗL�̓��͏���
/// </summary>
/// <param name="keys"></param>
void PlayerActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	// W�EA�ES�ED�ړ�
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400.0f;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);

	// �}�E�X�ړ�
	// SDL���瑊�Ή^�����擾
	int x, y;
	Uint32 buttons =  SDL_GetRelativeMouseState(&x, &y);
	// �}�E�X�̓����́A�ʏ�-500����+500�͈̔�
	const int maxMouseSpeed = 500;
	// �ő�ړ��ʂɂ�����p���x
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;

	if (x != 0)
	{
		// ���悻[-1.0,1.0]�͈̔͂ɕϊ�
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		// �ő�ړ��ʂł̊p���x���|����
		angularSpeed *= maxAngularSpeed;
	}

	mMoveComp->SetAngularSpeed(angularSpeed);
}

/// <summary>
/// �Փˏ���
/// </summary>
/// <param name="ownCollider"></param>
/// <param name="otherCollider"></param>
void PlayerActor::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}