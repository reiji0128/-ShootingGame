#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class HDR
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	HDR();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~HDR();
	
	/// <summary>
	/// HDR�����_�����O�̊J�n����
	/// </summary>
	void HDRRenderingBegin();

	/// <summary>
	/// HDR�����_�����O�̏I������
	/// </summary>
	void HDRRenderingEnd();

	/// <summary>
	/// ���������_�t���[���o�b�t�@�̍쐬
	/// </summary>
	void CreateHDRBuffer();

	/// <summary>
	/// �l�p�`�|���S���̍쐬
	/// </summary>
	void CreateQuadVAO();

	/// <summary>
	/// �l�p�`�|���S���̕`��
	/// </summary>
	void RenderQuad();

private:
	unsigned int mHdrFBO;
	unsigned int mRbo;
	unsigned int mFloatColorTexture;
	const unsigned int mBufferWidth = 1280;
	const unsigned int mBufferHeght = 768;

	// ���_�z��I�u�W�F�N�gID
	unsigned int mVertexArray;
	// ���_�o�b�t�@ID
	unsigned int mVertexBuffer;
	// �C���f�b�N�XID
	unsigned int mIndexBuffer;

	// �l�p�`�|���S����\������V�F�[�_�[
	class Shader* mScreenBufferShader;
};