#include "CubeMapComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"

CubeMapComponent::CubeMapComponent(Actor* owner)
	:Component(owner)
	,mVisible(true)
{
}

CubeMapComponent::~CubeMapComponent()
{
}

void CubeMapComponent::CreateTexture(const std::string& fileName)
{
	mTexture = new Texture();
	mTexture->LoadCubeMap(fileName);
}

void CubeMapComponent::Draw(Shader* shader)
{
	if (mVisible)
	{
		glDepthFunc(GL_LEQUAL);
		RENDERER->GetCubeMapVerts()->SetActive();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture->GetTextureID());

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}
}
