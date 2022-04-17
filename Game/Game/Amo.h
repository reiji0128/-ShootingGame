#pragma once
#include "Actor.h"
#include "Gun.h"

class Amo : public Actor
{
public:
	Amo(Tag tag, Gun* gun);

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Amo(Vector3& position,Tag tag,Gun* gun);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Amo();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="deltaTiem">1�t���[���̌o�ߎ���</param>
	void UpdateActor(float deltaTime)override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="ownCollider"></param>
	/// <param name="otherCollider"></param>
	void OnCollisionEnter(class ColliderComponent* ownCollider, class ColliderComponent* otherCollider)override;

	void ActiveAmo(Amo* amo);
private:

	void CreateAmo();

	// ������
	const int mCreateNum;

	// �ő�A�N�e�B�u��
	const int mMaxActive;

	// �A�N�e�B�u��
	int mActiveCount;

	// �A�N�e�B�u���ǂ����̃t���O
	bool mIsActive;

	// �e�̓��I�z��
	std::vector<Amo*> mAmoList;

	// Gun�N���X�̃|�C���^
	Gun* mGun;
};