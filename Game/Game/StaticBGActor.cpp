#include "StaticBGActor.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">位置</param>
/// <param name="gpmeshFileName">メッシュのファイルパス</param>
StaticBGActor::StaticBGActor(const Vector3& position, const char* gpmeshFileName)
	: Actor(Tag::BackGround)
{
	mPosition = position;
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mesh);
}

/// <summary>
/// デストラクタ
/// </summary>
StaticBGActor::~StaticBGActor()
{
}