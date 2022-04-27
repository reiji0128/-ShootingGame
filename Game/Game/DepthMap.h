#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Math.h"
#include "Shader.h"

class DepthMap
{
public:
	DepthMap();
	~DepthMap();
	void DepthRenderingBegin();
	void DepthRenderingEnd();
	void CreateShadowMap();
	void CalcLightSpaceMatrix(const Vector3& centerWorldPos,const Vector3& lightDir,const Vector3& upVec,const float lightDistance);

	// ゲッター //
	unsigned int GetDepthTexID() { return mDepthMap; }

	Vector3& GetLightDir() { return mLightDir; }

	Vector3& GetLightPod() { return mLightPos; }

	Matrix4& GetLightSpaceMatrix() { return mLightSpaceMatrix; }
private:
	// デプスマップフレームバッファオブジェクト
	unsigned int mDepthMapFBO;

	// デプステクスチャ
	unsigned int mDepthMap;

	const unsigned int SHADOW_WIDTH  = 4096;
	const unsigned int SHADOW_HEIGHT = 4096;

	//デプスマップレンダリングの撮影原点
	Vector3 mLightPos;

	// 光線ベクトル方向
	Vector3 mLightDir;

	// ライト空間
	Matrix4 mLightSpaceMatrix;
};