#include "Amo.h"
#include "Game.h"
#include "EffectComponent.h"
#include "BoxCollider.h"

const int speed = 50.0f;
const int shootRange = 100.0;

Amo::Amo(Tag tag, Gun* gun)
	:Actor(tag)
	, mCreateNum(31)
	, mMaxActive(31)
	, mGun(gun)
{
	CreateAmo();
}

Amo::Amo(Vector3& position,Tag tag,Gun* gun)
	:Actor(tag)
	,mCreateNum(31)
	,mMaxActive(31)
	,mGun(gun)
{
	mPosition = position;
	mSpeed = speed;

	// ’e“–‚½‚è”»’èƒGƒŠƒAì¬•BoxCollider“o˜^
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

Amo::~Amo()
{
}

void Amo::UpdateActor(float deltaTime)
{
	for (auto AmoItr : mAmoList)
	{
		printf("%d\n", mAmoList.size());
		if (AmoItr->GetState() == State::EActive && mGun->GetFireFlag())
		{
			// ’eƒGƒtƒFƒNƒg¶¬
			EffectComponent* ec = new EffectComponent(this, true, true);
			ec->LoadEffect(u"Assets/Effect/bullet.efk");

			AmoItr->mPosition += mDirection * mSpeed * deltaTime;
		}

		if (AmoItr->GetState() != State::EInactive && mActiveCount < mMaxActive)
		{
			ActiveAmo(AmoItr);
		}

		// ˆê’è‹——£—£‚ê‚½‚ç’e‚ðÁ‚·
		Vector3 length = AmoItr->mPosition - GAMEINSTANCE.GetPlayerActor()->GetPosition();;
		if (length.LengthSq() > shootRange * shootRange)
		{
			mState = Actor::EInactive;
		}

		mRecomputeWorldTransform = true;
	}
}

void Amo::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}

void Amo::ActiveAmo(Amo* amo)
{
	if (amo->GetState() != State::EActive)
	{
		amo->SetState(State::EActive);
		++mActiveCount;
	}
}

void Amo::CreateAmo()
{
	for (int i = 0; i < mCreateNum; i++)
	{
		// e‚ÌÀ•W‚ðŽæ“¾
		Vector3 position = GAMEINSTANCE.GetPlayerActor()->GetPosition();;
		mAmoList.push_back(new Amo(position, Tag::PlayerBullet,mGun));
		++mActiveCount;
	}
}
