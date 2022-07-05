#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class CubeMap
{
public:
	CubeMap();
	~CubeMap();

	/// <summary>
	/// �e�N�X�`���̐���
	/// </summary>
	void CreateTexture();

	/// <summary>
	/// �e�N�X�`�����^�[�Q�b�g��Ɋ��蓖�Ă�
	/// </summary>
	void AssignmentTexture();

private:
	// �e�N�X�`����ID
	unsigned int mTextureID;

	// �e�N�X�`���̍���
	int mTextureWidth;

	// �e�N�X�`���̕�
	int mTextureHeight;

	const char* mTextures;

	// �e�N�X�`���̃^�[�Q�b�g��
	const std::vector<GLenum> mTextureFace
	{
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	};
};