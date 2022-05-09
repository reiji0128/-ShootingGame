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
/// ���������_�t���[���o�b�t�@�̍쐬
/// </summary>
void HDR::CreateHDRBuffer()
{
	// FBO�Ɋ��蓖�Ă邽�߂̊k�̃e�N�X�`�����쐬
	glGenTextures(1, &mFloatColorTexture);
	glBindTexture(GL_TEXTURE_2D, mFloatColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mBufferWidth, mBufferHeght, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// FBO�ɃJ���[�e�N�X�`���Ƃ���frameColorTexture���A�^�b�`����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFloatColorTexture, 0);

	// �����_�[�o�b�t�@�[�̍쐬
	glGenRenderbuffers(1, &mRbo);
	glBindRenderbuffer(GL_RENDERBUFFER,mRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mBufferWidth, mBufferHeght);

	// FBO�Ƀ����_�[�o�b�t�@�[���A�^�b�`����
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
	}
	// �f�t�H���g�ɖ߂�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
