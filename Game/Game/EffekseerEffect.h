#pragma once

#include <string>
#include <Effekseer.h>
#include "Math.h"

class EffekseerEffect
{
public:
	EffekseerEffect();
	~EffekseerEffect();
	bool LoadEffect(const char16_t* filename);
	bool IsReady() { return mIsReady; }
	Effekseer::Handle    CreateInstanceHandle(Vector3& position);

	static void SetPosition(Vector3& pos, Effekseer::Handle handle);
	static void SetRotation(Vector3& axis, float angle, Effekseer::Handle handle);
	static void SetBaseMatrix(Matrix4& baseMat, Effekseer::Handle handle);

private:
	bool                 mIsReady;
	Effekseer::EffectRef mEffectRef;
	Effekseer::Handle    mHandle;
};


