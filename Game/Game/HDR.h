#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class HDR
{
public:
	HDR();
	~HDR();
	void HDRRenderingBegin();
	void HDRRenderingEnd();
	/// <summary>
	/// 浮動小数点フレームバッファの作成
	/// </summary>
	void CreateHDRBuffer();
	
private:
	unsigned int mHdrFBO;
	unsigned int mRbo;
	unsigned int mFloatColorTexture;
	const unsigned int mBufferWidth = 1024;
	const unsigned int mBufferHeght = 1024;

};