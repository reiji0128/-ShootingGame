#include "Altar.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "LineCollider.h"

Altar::Altar(const Vector3& pos, const char* gpmeshFileName)
	:Actor(Tag::BackGround)
	, mBoxCollider(nullptr)
	, mLineCollider(nullptr)
{
	mPosition = pos;
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mesh);

	// 当たり判定のセット
	AABB box = mesh->GetCollisionBox();
	box.Scaling(1.0f);
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);

	// ラインコライダーのセット
	mLineCollider = new LineCollider(this);
	Line line(Vector3(0, 0, 50.0f), Vector3(0, 0, -20.0f));
	mLineCollider->SetLine(line);
}

Altar::~Altar()
{
}

void Altar::UpdateActor(float deltaTime)
{
}

void Altar::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}
