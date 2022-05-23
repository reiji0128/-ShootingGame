#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Math.h"

class HDR
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HDR();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~HDR();
	
	/// <summary>
	/// HDRレンダリングの開始処理
	/// </summary>
	void HDRRenderingBegin();

	/// <summary>
	/// HDRレンダリングの終了処理
	/// </summary>
	void HDRRenderingEnd();

	/// <summary>
	/// 浮動小数点フレームバッファの作成
	/// </summary>
	void CreateHDRBuffer();

	/// <summary>
	/// 縮小バッファの作成(ダウンサンプリング&ぼかし用のためのフレームバッファ)
	/// </summary>
	void CreateScaleDownBuffer();

	/// <summary>
	/// 縮小バッファパス
	/// </summary>
	void ScaleDownBufferPath();

	/// <summary>
	/// ガウスぼかしの重み係数計算
	/// </summary>
	/// <param name="pos">フラグメントの位置</param>
	/// <param name="rho">偏差(大きくするとやまがなだらかになる)</param>
	/// <returns>ガウスぼかしの重み係数</returns>
	float GaussianDistribution(const Vector2& pos, float rho);

	/// <summary>
	/// ガウスぼかし曲線関数
	/// </summary>
	void CalcGaussBlurParam(int w, int h, Vector2 dir,float deviation, Vector3* offset);

	/// <summary>
	/// 四角形ポリゴンの作成
	/// </summary>
	void CreateQuadVAO();

	/// <summary>
	/// 四角形ポリゴンの描画
	/// </summary>
	void RenderQuad();

private:
	unsigned int mHdrFBO;
	unsigned int mRbo;
	unsigned int mFloatColorTexture;

	unsigned int mColorBuffers[2];

	// 縮小バッファレベル数
	unsigned int mMaxLevelNum;

	const unsigned int mBufferWidth = 1280;
	const unsigned int mBufferHeght = 768;

	// 頂点配列オブジェクトID
	unsigned int mVertexArray;
	// 頂点バッファID
	unsigned int mVertexBuffer;
	// インデックスID
	unsigned int mIndexBuffer;

	// FBOとテクスチャID確保用
	std::vector<unsigned int> mBlurFBO;
	std::vector<unsigned int> mBlurBufferTex;

	// 四角形ポリゴンを表示するシェーダー
	class Shader* mScreenBufferShader;

	// ダウンサンプリングシェーダー
	class Shader* mDownSamplingShader;

	// ガウスシェーダー
	class Shader* mGaussShader;
};