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

	// ’eƒGƒtƒFƒNƒg¶¬
	EffectComponent* ec = new EffectComponent(this, true, true);
	ec->LoadEffect(u"Assets/Effect/bullet.efk");

	// ’e“–‚½‚è”»’èƒGƒŠƒAì¬•BoxCollider“o˜^
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

	// ˆê’è‹——£—£‚ê‚½‚ç’e‚ğÁ‚·
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

	// Õ“Ëî•ñ
	CollisionInfo info;

	// ”wŒi‚ÆÕ“Ë‚µ‚½‚©
	if (colliderTag == Tag::BackGround)
	{
		// ”wŒiBox‚ÉÕ“Ë‚µ‚½‚Ì‚àBoxH
		if (ownCollider->GetColliderType() == ColliderTypeEnum::Box
			|| colliderTag == Tag::Enemy)
		{
			//ƒqƒbƒgƒ{ƒbƒNƒXH
			if (ownCollider == mHitBox)
			{
				mState = State::EDead;
			}
		}
	}
}