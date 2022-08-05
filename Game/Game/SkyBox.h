#pragma once
#include "Actor.h"

class SkyBox : public Actor
{
public:
	SkyBox();

	~SkyBox();

private:
	class CubeMapComponent* mCubeMapComp;
};