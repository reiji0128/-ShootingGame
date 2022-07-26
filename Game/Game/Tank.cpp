#include "Tank.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"

Tank::Tank(const Vector3& position, const char* gpmeshFileName)
	:Actor(Tag::BackGround)
	,mShaderTag(ShaderTag::Normal)
{
	mPosition = position;
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName);
	mesh->SetUseNormalMap(true);
	MeshComponent* mc = new MeshComponent(this, mShaderTag);
	mc->SetMesh(mesh);
}

Tank::~Tank()
{
}
