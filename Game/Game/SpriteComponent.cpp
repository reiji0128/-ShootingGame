#include "SpriteComponent.h"
#include "Game.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mTexture(nullptr)
	,mTexWidth(0)
	,mTexHeight(0)
{
	GAMEINSTANCE.GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	GAMEINSTANCE.GetRenderer()->RemoveSprite(this);
	
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture)
	{
		//クワッドをテクスチャの幅/高さでスケーリングします
		Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexWidth),
												static_cast<float>(mTexHeight),
												1.0f);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		//すべてのスプライトは同じシェーダー/頂点を使用するため、
		//スプライトが描画する前に、ゲームは最初にそれらをアクティブに設定します

		//ワールド座標変換をセット
		shader->SetMatrixUniform("uWorldTransform", world);
		//現在のテクスチャをセット
		mTexture->SetActive();
		//クワッドを描画
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	//幅・高さをセット
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
