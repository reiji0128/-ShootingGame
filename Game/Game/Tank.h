#pragma once
#include "Actor.h"
#include "Math.h"
#include "ShaderTag.h"

class Tank : public Actor
{
public:
	Tank(const Vector3& position, const char* gpmeshFileName);

	~Tank();

private:
	ShaderTag mShaderTag;
};