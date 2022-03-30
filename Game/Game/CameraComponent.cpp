#include "CameraComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Game.h"
#include "AudioManager.h"

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
{
}

void CameraComponent::SetViewMatrix(const Matrix4& view)
{
	// ビュー行列をレンダラーとオーディオシステムに渡す
	GAMEINSTANCE.GetRenderer()->SetViewMatrix(view);
}
