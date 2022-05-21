#include "HDR.h"
#include "Shader.h"
#include <iostream>

/// <summary>
/// �R���X�g���N�^
/// </summary>
HDR::HDR()
{
	//�������_�t���[���o�b�t�@�̍쐬
	CreateHDRBuffer();
	// �l�p�`�|���S���̍쐬
	CreateQuadVAO();

	// �o�b�t�@�[�V�F�[�_�[�̃��[�h
	mScreenBufferShader = new Shader();
	if (!mScreenBufferShader->Load("Shaders/ScreenBuffer.vert", "Shaders/HDR.frag"))
	{
		printf("�o�b�t�@�[�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
HDR::~HDR()
{
}

/// <summary>
/// HDR�����_�����O�̊J�n����
/// </summary>
void HDR::HDRRenderingBegin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mHdrFBO);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// <summary>
/// HDR�����_�����O�̏I������
/// </summary>
void HDR::HDRRenderingEnd()
{
	// �`��Ώۂ��X�N���[���֖߂�
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

/// <summary>
/// ���������_�t���[���o�b�t�@�̍쐬
/// </summary>
void HDR::CreateHDRBuffer()
{
	glGenFramebuffers(1, &mHdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mHdrFBO);

	// FBO�Ɋ��蓖�Ă邽�߂̋�̃e�N�X�`�����쐬
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

/// <summary>
/// �l�p�`�|���S���̍쐬
/// </summary>
void HDR::CreateQuadVAO()
{
	float vertices[] =
	{
		//   �ʒu      |    uv���W
		-1.0f,  1.0f,     0.0f, 1.0f,       // ����  0
		-1.0f, -1.0f,     0.0f, 0.0f,       // ����  1
		 1.0f,  1.0f,     1.0f, 1.0f,       // �E��  2
		 1.0f, -1.0f,     1.0f, 0.0f,       // �E��  3
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,3,1
	};

	// ���_�z��̍쐬
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// ���_�o�b�t�@�̍쐬
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArray);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Attribute 0 �ʒu
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	// Attribute 1 uv���W
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(sizeof(float) * 2));
}

/// <summary>
/// �l�p�`�|���S���̕`��
/// </summary>
void HDR::RenderQuad()
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mScreenBufferShader->SetActive();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFloatColorTexture);
	mScreenBufferShader->SetIntUniform("uHDRBuffer", 0);
	mScreenBufferShader->SetFloatUniform("exposure", 0.01);
	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}