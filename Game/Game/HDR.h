#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
	const unsigned int mBufferWidth = 1280;
	const unsigned int mBufferHeght = 768;

	// 頂点配列オブジェクトID
	unsigned int mVertexArray;
	// 頂点バッファID
	unsigned int mVertexBuffer;
	// インデックスID
	unsigned int mIndexBuffer;

	// 四角形ポリゴンを表示するシェーダー
	class Shader* mScreenBufferShader;
};