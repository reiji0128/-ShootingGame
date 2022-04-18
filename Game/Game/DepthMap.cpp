#include "DepthMap.h"

DepthMap::DepthMap()
{
}

DepthMap::~DepthMap()
{
}

void DepthMap::CreateDepthTexture()
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

	// デプステクスチャをデプスバッファとしてアタッチ

}
