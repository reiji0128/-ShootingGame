#include "GBuffer.h"
#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include "Shader.h"

GBuffer::GBuffer()
{
	mScreenWidth = RENDERER->GetScreenWidth();
	mScreenHeight = RENDERER->GetScreenHeight();
}

GBuffer::~GBuffer()
{
}

void GBuffer::GBufferRenderingBegin()
{
	// 描画先をGbufferにセット
	glBindFramebuffer(GL_FRAMEBUFFER, mGbuffer);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::GBufferRenderingEnd()
{
	// 描画先をスクリーンに戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::CreateGbuffer()
{
	glGenFramebuffers(1, &mGbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mGbuffer);
	{
		// 位置用カラーバッファ
		glGenTextures(1, &mPosition);
		glBindTexture(GL_TEXTURE_2D, mPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mScreenWidth, mScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPosition, 0);

		// 法線用カラーバッファ
		GLint i;
		glGenTextures(1, &mNormal);
		glBindTexture(GL_TEXTURE_2D, mNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mScreenWidth, mScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormal, 0);

		// アルベド(RGB) & スペキュラー(A)用カラーバッファ
		glGenTextures(1, &mAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, mAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mScreenWidth, mScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mAlbedoSpec, 0);

		// レンダリングに使用するカラーバッファのアタッチメント
		unsigned int attachments[3] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
		};

		glDrawBuffers(3, attachments);

		// レンダーバッファーの作成
		glGenRenderbuffers(1, &mRenderBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mScreenWidth, mScreenHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferObject);
		
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			printf("FB error, status: 0x%x\n", Status);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::InputGBufferToShader(Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mPosition);
	//shader->SetIntUniform("gPosition"  , 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mNormal);
	//shader->SetIntUniform("gNormal"    , 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mAlbedoSpec);
	//shader->SetIntUniform("gAlbedoSpec", 2);
}