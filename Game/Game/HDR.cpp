#include "HDR.h"
#include "Shader.h"
#include <iostream>

/// <summary>
/// コンストラクタ
/// </summary>
HDR::HDR()
{
	//動小数点フレームバッファの作成
	CreateHDRBuffer();
	// 四角形ポリゴンの作成
	CreateQuadVAO();

	// バッファーシェーダーのロード
	mScreenBufferShader = new Shader();
	if (!mScreenBufferShader->Load("Shaders/ScreenBuffer.vert", "Shaders/HDR.frag"))
	{
		printf("バッファーシェーダーの読み込み失敗\n");
	}
}

/// <summary>
/// デストラクタ
/// </summary>
HDR::~HDR()
{
}

/// <summary>
/// HDRレンダリングの開始処理
/// </summary>
void HDR::HDRRenderingBegin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mHdrFBO);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// <summary>
/// HDRレンダリングの終了処理
/// </summary>
void HDR::HDRRenderingEnd()
{
	// 描画対象をスクリーンへ戻す
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

/// <summary>
/// 浮動小数点フレームバッファの作成
/// </summary>
void HDR::CreateHDRBuffer()
{
	glGenFramebuffers(1, &mHdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mHdrFBO);

	// FBOに割り当てるための空のテクスチャを作成
	glGenTextures(1, &mFloatColorTexture);
	glBindTexture(GL_TEXTURE_2D, mFloatColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mBufferWidth, mBufferHeght, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// FBOにカラーテクスチャとしてframeColorTextureをアタッチする
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFloatColorTexture, 0);

	// レンダーバッファーの作成
	glGenRenderbuffers(1, &mRbo);
	glBindRenderbuffer(GL_RENDERBUFFER,mRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mBufferWidth, mBufferHeght);

	// FBOにレンダーバッファーをアタッチする
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
	}
	// デフォルトに戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
/// 四角形ポリゴンの作成
/// </summary>
void HDR::CreateQuadVAO()
{
	float vertices[] =
	{
		//   位置      |    uv座標
		-1.0f,  1.0f,     0.0f, 1.0f,       // 左上  0
		-1.0f, -1.0f,     0.0f, 0.0f,       // 左下  1
		 1.0f,  1.0f,     1.0f, 1.0f,       // 右上  2
		 1.0f, -1.0f,     1.0f, 0.0f,       // 右下  3
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,3,1
	};

	// 頂点配列の作成
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// 頂点バッファの作成
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArray);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	// インデックスバッファの作成
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Attribute 0 位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	// Attribute 1 uv座標
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(sizeof(float) * 2));
}

/// <summary>
/// 四角形ポリゴンの描画
/// </summary>
void HDR::RenderQuad()
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mScreenBufferShader->SetActive();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFloatColorTexture);
	mScreenBufferShader->SetIntUniform("uHDRBuffer", 0);
	mScreenBufferShader->SetFloatUniform("exposure", 0.01);
	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}