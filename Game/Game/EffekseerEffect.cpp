#include "EffekseerEffect.h"
#include "Game.h"
#include "Renderer.h"
#include "Effekseer.h"
#include "EffekseerRendererGL.h"
#include <string>
#include <iostream>

EffekseerEffect::EffekseerEffect()
	: mEffectRef(nullptr)
	, mHandle(-1)
	, mIsReady(false)
{
}

EffekseerEffect::~EffekseerEffect()
{

}

// エフェクトの読み込み
bool EffekseerEffect::LoadEffect(const char16_t* filename)
{
	std::cout << filename;

	mEffectRef = Effekseer::Effect::Create(RENDERER->GetEffekseerManager(), filename);
	if (mEffectRef == nullptr)
	{
		std::cout << " : Effect File Load Failed." << std::endl;
		mIsReady = false;
	}
	else
	{
		std::cout << " : File Load Success." << std::endl;
		mIsReady = true;
	}
	return mIsReady;
}

// エフェクトインスタンスハンドルの作成
Effekseer::Handle EffekseerEffect::CreateInstanceHandle(Vector3& pos)
{
	if (!mIsReady)
	{
		return 0;
	}
	mHandle = RENDERER->GetEffekseerManager()->Play(mEffectRef, Effekseer::Vector3D(pos.x, pos.y, pos.z));
	return mHandle;
}

// エフェクト位置のセット
void EffekseerEffect::SetPosition(Vector3& pos, Effekseer::Handle handle)
{
	Effekseer::Vector3D v;
	v = pos;
	RENDERER->GetEffekseerManager()->SetLocation(handle, v);
}

void EffekseerEffect::SetRotation(Vector3& axis, float angle, Effekseer::Handle handle)
{
	Effekseer::Vector3D v;
	v = axis;
	RENDERER->GetEffekseerManager()->SetRotation(handle, v, angle);
}

void EffekseerEffect::SetBaseMatrix(Matrix4& baseMat, Effekseer::Handle handle)
{
	Effekseer::Matrix43 m;
	m = baseMat;
	RENDERER->GetEffekseerManager()->SetBaseMatrix(handle, m);
}
