#include "HDR.h"
#include <iostream>

HDR::HDR()
{
}

HDR::~HDR()
{
}

void HDR::HDRRenderingBegin()
{
	
}

void HDR::HDRRenderingEnd()
{
}

/// <summary>
/// 浮動小数点フレームバッファの作成
/// </summary>
void HDR::CreateHDRBuffer()
{
	// FBOに割り当てるための殻のテクスチャを作成
	glGenTextures(1, &mFloatColorTexture);
	glBindTexture(GL_TEXTURE_2D, mFloatColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mBufferWidth, mBufferHeght, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// FBOにカラーテクスチャとしてframeColorTextureをアタッチする
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFloatColorTexture, 0);

	// レンダーバッファーの作成
	glGenRenderbuffers(1, &mRbo);
	glBindRenderbuffer(GL_RENDERBUFFER,mRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mBufferWidth, mBufferHeght);

	// FBOにレンダーバッファーをアタッチする
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
	}
	// デフォルトに戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
