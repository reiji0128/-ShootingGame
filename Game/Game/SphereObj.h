#pragma once
#include "Actor.h"
#include "Math.h"
#include "ShaderTag.h"

class SphereObj : public Actor
{
public:
	SphereObj(const Vector3& pos);
	~SphereObj();

private:
	ShaderTag mShaderTag;
};