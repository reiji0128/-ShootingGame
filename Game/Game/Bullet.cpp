#include "Bullet.h"
#include "EffectComponent.h"
#include "BoxCollider.h"

BulletActor::BulletActor(const Vector3& pos, const Vector3& dir, Tag type, float speed, float range)
	:Actor(type)
	, mStartPos(pos)
	, mShootRange(range)
{
	mPosition = pos;
	mDirection = dir;
	mSpeed = speed;

	// �e�G�t�F�N�g����
	EffectComponent* ec = new EffectComponent(this, true, true);
	ec->LoadEffect(u"Assets/Effect/bullet.efk");

	// �e�����蔻��G���A�쐬��BoxCollider�o�^
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

BulletActor::~BulletActor()
{
}

void BulletActor::UpdateActor(float deltaTime)
{
	mPosition += mSpeed * deltaTime * mDirection;

	// ��苗�����ꂽ��e������
	Vector3 length = mPosition - mStartPos;
	if (length.LengthSq() > mShootRange * mShootRange)
	{
		mState = Actor::EDead;
	}

	mRecomputeWorldTransform = true;
}

void BulletActor::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
	Tag colliderTag = otherCollider->GetTag();

	// �Փˏ��
	CollisionInfo info;

	// �w�i�ƏՓ˂�����
	if (colliderTag == Tag::BackGround)
	{
		// �w�iBox�ɏՓ˂����̂�Box�H
		if (ownCollider->GetColliderType() == ColliderTypeEnum::Box
			|| colliderTag == Tag::Enemy)
		{
			//�q�b�g�{�b�N�X�H
			if (ownCollider == mHitBox)
			{
				mState = State::EDead;
			}
		}
	}
}