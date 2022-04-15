#pragma once
#include "Actor.h"
#include "SkeletalMeshComponent.h"

class Gun : public Actor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�I�t�Z�b�g�ʒu</param>
	/// <param name="rot">�I�t�Z�b�g��]</param>
	/// <param name="gpmeshFileName">gpmesh�̃t�@�C���p�X</param>
	/// <param name="skelComp">SkeletalMeshComponent�̃|�C���^</param>
	/// <param name="boneName">�����������{�[����</param>
	Gun(const Vector3& pos,const Vector3& rot,
	    const char* gpmeshFileName,SkeletalMeshComponent* skelComp,
		const char* boneName);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Gun();

private:
	// �A�^�b�`��̃{�[����
	const char* mBoneName;

	// SkeletalMeshComponent�N���X�̃|�C���^
	class SkeletalMeshComponent* mSkelComp;

	// AttachMeshComponent�N���X�̃|�C���^(����̃A�^�b�`�����g�p)
	class AttachMeshComponent* mAttachComp;
};