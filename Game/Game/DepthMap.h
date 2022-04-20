#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Math.h"

class DepthMap
{
public:
	DepthMap();
	~DepthMap();
	void DepthRenderingBegin();
	void CreateShadowMap();
	void CalcLightSpaceMatrix(const Vector3& centerWorldPos,const Vector3& lightDir,const Vector3& upVec,const float lightDistance);
private:
	// デプスマップフレームバッファオブジェクト
	unsigned int mDepthMapFBO;

	// デプステクスチャ
	unsigned int mDepthMap;

	const unsigned int SHADOW_WIDTH  = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;

	//デプスマップレンダリングの撮影原点
	Vector3 mLightPos;

	// 光線ベクトル方向
	Vector3 mLightDir;

	// ライト空間
	Matrix4 mLightSpaceMatrix;

	// デプスマップ用シェーダー
	Shader* mDepthShader;
};