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

// �Q�b�^�[ //
	unsigned int GetHDRFrameBuffer() { return mFloatColorTexture; }
private:
	unsigned int mHdrFBO;
	unsigned int mRbo;
	unsigned int mFloatColorTexture;
	const unsigned int mBufferWidth = 1024;
	const unsigned int mBufferHeght = 1024;

};