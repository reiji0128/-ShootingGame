#include "DepthMap.h"
#include "Game.h"
#include "Renderer.h"

DepthMap::DepthMap()
{
	mDepthShader = new Shader;
	mDepthShader->Load("Shaders/DepthMap.vert", "Shaders/DepthMap.frag");
}

DepthMap::~DepthMap()
{
}

void DepthMap::DepthRenderingBegin()
{
	// 描画先をデプスマップに設定しシェーダーをセットする
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	mDepthShader->SetActive();
	mDepthShader->SetMatrixUniform("lightSpaceMatrix", mLightSpaceMatrix);
	
}

void DepthMap::DepthRenderingEnd()
{
	// 描画先をスクリーンに戻す
	glViewport(0, 0, RENDERER->GetScreenWidth(), RENDERER->GetScreenHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
}

void DepthMap::CreateShadowMap()
{
	// 深度マップをレンダリングするためのフレームバッファを作成
	glGenFramebuffers(1, &mDepthMapFBO);

	// 深度バッファとして使う2Dテクスチャを作成
	glGenTextures(1, &mDepthMap);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// mDepthMapFBOにデプステクスチャをアタッチ
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

/// <summary>
/// ライト空間の計算
/// </summary>
/// <param name="centerWorldPos"></param>
/// <param name="lightDir"></param>
/// <param name="upVec"></param>
/// <param name="lightDistance"></param>
void DepthMap::CalcLightSpaceMatrix(const Vector3& centerWorldPos, const Vector3& lightDir, const Vector3& upVec, const float lightDistance)
{
	mLightDir = lightDir;
	mLightPos = (-lightDistance) * mLightDir + centerWorldPos;

	Matrix4 lightProjection = Matrix4::CreateOrtho(7500, 7500, 1.0f, 10000.0f);
	Matrix4 lightView       = Matrix4::CreateLookAt(mLightPos,centerWorldPos,upVec);

	mLightSpaceMatrix = lightView * lightProjection;
}
