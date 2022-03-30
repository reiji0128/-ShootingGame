#include "EffectComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Effekseer.h"
#include "EffekseerEffect.h"

EffectComponent::EffectComponent(Actor* owner, bool moveOn, bool rotateOn, int updateOrder)
	: Component(owner)
	, mOwner(owner)
	, mIsMove(moveOn)
	, mIsRotate(rotateOn)
	, mHandle(0)
	, mRelativePos(0, 0, 0)
{
}


EffectComponent::~EffectComponent()
{

	RENDERER->GetEffekseerManager()->StopEffect(mHandle);
}

void EffectComponent::LoadEffect(const char16_t* effkseerFilename)
{
	EffekseerEffect* effect = RENDERER->GetEffect(effkseerFilename);

	Vector3 pos = Vector3(0, 0, 0);
	mHandle = effect->CreateInstanceHandle(pos);
}

void EffectComponent::Update(float deltaTime)
{
	// エフェクトが現在も生きているか？
	if (!(RENDERER->GetEffekseerManager()->Exists(mHandle)))
	{
		mOwner->SetState(Actor::EDead);
		return;
	}

	// エフェクトを移動させるか？
	Matrix4 trans, rot;
	if (mIsMove)
	{
		Vector3 ownerPos;
		ownerPos = mOwner->GetPosition();
		trans = Matrix4::CreateTranslation(ownerPos);
	}
	// エフェクトはオーナーの回転に影響を受けるか
	if (mIsRotate)
	{
		Quaternion q = mOwner->GetRotation();
		rot = Matrix4::CreateFromQuaternion(q);
	}
	Effekseer::Matrix43 eMat;

	// Effecseer -> GL の100倍 + Zup に合わせる
	Matrix4 base, convertScale, convertRot;
	convertScale = Matrix4::CreateScale(100, -100, 100);
	convertRot = Matrix4::CreateRotationX(Math::ToRadians(-90.0f));
	base = convertScale * convertRot * mRelativeRot;

	//エフェクトに移動・回転（あれば）セット
	Matrix4 relativeMoveMat = Matrix4::CreateTranslation(mRelativePos);
	Matrix4 final;
	final = base * relativeMoveMat * rot * trans;

	final.mat[0][0] *= -1;
	final.mat[1][0] *= -1;
	final.mat[2][0] *= -1;
	final.mat[3][0] *= -1;

	eMat = final;
	RENDERER->GetEffekseerManager()->SetBaseMatrix(mHandle, eMat);

}
