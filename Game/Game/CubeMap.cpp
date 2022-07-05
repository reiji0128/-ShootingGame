#include "CubeMap.h"
CubeMap::CubeMap()
{
}

CubeMap::~CubeMap()
{
}

void CubeMap::CreateTexture()
{
	// テクスチャの生成
	glGenTextures(1, &mTextureID);
	// テクスチャターゲットにバインド
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
}

void CubeMap::AssignmentTexture()
{
	for (int i = 0; i < mTextureFace.size(); i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB,mTextureWidth,mTextureHeight,0,GL_RGB,GL_UNSIGNED_BYTE)
	}
}
