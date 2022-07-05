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
	/// テクスチャの生成
	/// </summary>
	void CreateTexture();

	/// <summary>
	/// テクスチャをターゲット先に割り当てる
	/// </summary>
	void AssignmentTexture();

private:
	// テクスチャのID
	unsigned int mTextureID;

	// テクスチャの高さ
	int mTextureWidth;

	// テクスチャの幅
	int mTextureHeight;

	const char* mTextures;

	// テクスチャのターゲット先
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