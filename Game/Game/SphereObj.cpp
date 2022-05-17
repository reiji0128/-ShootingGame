#include "SphereObj.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"


SphereObj::SphereObj(const Vector3& pos)
	:Actor(Tag::Altar)
	, mShaderTag(ShaderTag::HDR)
{
	mPosition = pos;
	Mesh* mesh = RENDERER->GetMesh("Assets/Sphere/Sphere.gpmesh");
	MeshComponent* mc = new MeshComponent(this, mShaderTag);
	mc->SetMesh(mesh);
}

SphereObj::~SphereObj()
{
}
