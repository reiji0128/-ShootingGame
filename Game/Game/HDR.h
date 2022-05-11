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

// ゲッター //
	unsigned int GetHDRFrameBuffer() { return mFloatColorTexture; }
private:
	unsigned int mHdrFBO;
	unsigned int mRbo;
	unsigned int mFloatColorTexture;
	const unsigned int mBufferWidth = 1024;
	const unsigned int mBufferHeght = 1024;

};