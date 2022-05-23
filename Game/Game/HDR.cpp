#include "HDR.h"
#include "Shader.h"
#include <iostream>

/// <summary>
/// コンストラクタ
/// </summary>
HDR::HDR()
{
	// 動小数点フレームバッファの作成
	CreateHDRBuffer();
	// 縮小バッファの作成
	CreateScaleDownBuffer();
	// 四角形ポリゴンの作成
	CreateQuadVAO();

	// スクリーンバッファーシェーダーのロード
	mScreenBufferShader = new Shader();
	if (!mScreenBufferShader->Load("Shaders/ScreenBuffer.vert", "Shaders/HDR.frag"))
	{
		printf("バッファーシェーダーの読み込み失敗\n");
	}

	// ダウンサンプリングシェーダーのロード
	mDownSamplingShader = new Shader();
	if (!mDownSamplingShader->Load("Shaders/ScreenBuffer.vert", "Shaders/DownSampling.frag"))
	{
		printf("ダウンサンプリングシェーダーの読み込み失敗\n");
	}

	// ガウスシェーダーのロード
	mGaussShader = new Shader();
	if (!mGaussShader->Load("Shaders/ScreenBuffer.vert", "Shaders/GaussBlur.frag"))
	{
		printf("ガウスシェーダーの読み込み失敗\n");
	}

	// HDRBloomブレンドシェーダーのロード
	mHDRBloomBlendShader = new Shader();
	if (!mHDRBloomBlendShader->Load("Shaders/ScreenBuffer.vert", "Shaders/HDRBloomBlend.frag"))
	{
		printf("HDRBloomブレンドシェーダーの読み込み失敗\n");
	}
}

/// <summary>
/// デストラクタ
/// </summary>
HDR::~HDR()
{
	delete mScreenBufferShader;
	delete mDownSamplingShader;
	delete mGaussShader;
	delete mHDRBloomBlendShader;
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
/// 浮動小数点フレームバッファと高輝度バッファの作成
/// </summary>
void HDR::CreateHDRBuffer()
{
	glGenFramebuffers(1, &mHdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mHdrFBO);

	glGenTextures(2, mColorBuffers);

	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, mColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mBufferWidth, mBufferHeght, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// フレームバッファにカラーテクスチャとしてcolorBufferをアタッチ
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mColorBuffers[i], 0);
	}


	// レンダーバッファーの作成
	glGenRenderbuffers(1, &mRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, mRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mBufferWidth, mBufferHeght);

	// FBOにレンダーバッファーをアタッチする
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbo);

	unsigned int attachments[2] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1
	};

	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
	}
	// デフォルトに戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
/// 縮小バッファの作成(ダウンサンプリング&ぼかし用のためのフレームバッファ)
/// </summary>
void HDR::CreateScaleDownBuffer()
{
	mMaxLevelNum = 5;         // 縮小バッファレベル数(1/2, 1/4, 1/8, 1/16, 1/32 5の段階)

	int w = mBufferWidth;
	int h = mBufferHeght;

	float borderColor[4] = { 0.0f };

	// FBOとテクスチャID確保用
	mBlurFBO.resize(mMaxLevelNum * 2);
	mBlurBufferTex.resize(mMaxLevelNum * 2);

	// 横方向・縦方向ブラー作成
	// mmMaxLevelNumの2倍分の枚数作成
	for (unsigned int i = 0; i < mMaxLevelNum; i++)
	{
		// 縮小バッファの幅・高さの計算
		w /= 2;
		h /= 2;

		for (int j = 0; j < 2; j++)
		{
			glGenFramebuffers(1, &mBlurFBO[i * 2 + j]);
			glGenTextures(1, &mBlurBufferTex[i * 2 + j]);
			{
				glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[i * 2 + j]);
				glBindTexture(GL_TEXTURE_2D, mBlurBufferTex[i * 2 + j]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurBufferTex[i * 2 + j], 0);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}

/// <summary>
/// 縮小バッファパス
/// </summary>
void HDR::ScaleDownBufferPath()
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	unsigned int renderSource = mColorBuffers[1];     // ダウンサンプリングさせる対象を輝度バッファに指定

	// 縮小バッファ1,3,5,7,9にコピー
	int reduceX = mBufferWidth;
	int reduceY = mBufferHeght;

	// ダウンサンプリング 1,3,5 ...と奇数番にダウンサンプリング結果を出力
	for (int i = 0; i < mMaxLevelNum; i++)
	{
		reduceX /= 2;
		reduceY /= 2;

		// 描画FBOに奇数番を指定
		glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[i * 2 + 1]);
		{
			glViewport(0, 0, reduceX, reduceY);

			// カラーバッファのクリア
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderSource);
			mDownSamplingShader->SetActive();
			mDownSamplingShader->SetIntUniform("uScreenTexture", 0);

			// スクリーン全体に描画
			glBindVertexArray(mVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		renderSource = mBlurBufferTex[i * 2 + 1];     // 描画終わったらテクスチャを次回にダウンサンプリングの対象に
	}

	// ガウスぼかし処理 //
	const unsigned int sampleCount = 15;
	Vector3 offset[sampleCount];

	reduceX = mBufferWidth;
	reduceY = mBufferHeght;
	float deviation = 5.0f;
	renderSource = mBlurBufferTex[1];

	// ガウスレベル数分
	for (int i = 0; i < mMaxLevelNum; i++)
	{
		reduceX /= 2;
		reduceY /= 2;

		// horizontal  0 : 水平  1 : 垂直方向  にガウスぼかしをかける
		for (int horizontal = 0; horizontal < 2; horizontal++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[i * 2 + horizontal]);
			{
				glViewport(0, 0, reduceX, reduceY);
				Vector2 dir;
				if (horizontal)
				{
					dir = Vector2(0, 1);
				}
				else
				{
					dir = Vector2(1, 0);
				}
				CalcGaussBlurParam(reduceX, reduceY, dir, deviation, offset);

				// カラーバッファーのクリア
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, renderSource);
				mGaussShader->SetActive();
				mGaussShader->SetIntUniform("uBlursource", 0);
				mGaussShader->SetIntUniform("uParam.SampleCount", 15);

				// ガウスシェーダーにOffsetをセット
				for (int i = 0; i < sampleCount; i++)
				{
					std::string s = "uParam.Offset[" + std::to_string(i) + "]";
					mGaussShader->SetVectorUniform(s.c_str(), offset[i]);
				}

				// スクリーン全体に描画
				glBindVertexArray(mVertexArray);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

				renderSource = mBlurBufferTex[i * 2 + horizontal];         // 描画終わったらテクスチャを次回にダウンサンプリングの対象に
			}
		}
		// 回数ごとに偏差を上げる
		deviation *= deviation;
	}
}

/// <summary>
/// ガウスぼかしの重み係数計算
/// </summary>
/// <param name="pos">フラグメントの位置</param>
/// <param name="rho">偏差(大きくするとやまがなだらかになる)</param>
/// <returns>ガウスぼかしの重み係数</returns>
float HDR::GaussianDistribution(const Vector2& pos, float rho)
{
	return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
}

/// <summary>
/// ガウスぼかし曲線関数
/// </summary>
void HDR::CalcGaussBlurParam(int w, int h, Vector2 dir, float deviation, Vector3* offset)
{
	auto tu = 1.0f / float(w);
	auto tv = 1.0f / float(h);

	offset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), deviation);
	auto total_weight = offset[0].z;

	offset[0].x = 0.0f;
	offset[0].y = 0.0f;

	for (auto i = 1; i < 8; ++i)
	{
		int nextpos = (i - 1) * 2 + 1;
		offset[i].x = dir.x * tu * nextpos;
		offset[i].y = dir.y * tv * nextpos;
		offset[i].z = GaussianDistribution(dir * float(nextpos), deviation);
		total_weight += offset[i].z * 2.0f;
	}

	for (auto i = 0; i < 8; ++i)
	{
		offset[i].z /= total_weight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		offset[i].x = -offset[i - 7].x;
		offset[i].y = -offset[i - 7].y;
		offset[i].z = offset[i - 7].z;
	}
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mScreenBufferShader->SetActive();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mColorBuffers[0]);
	mScreenBufferShader->SetIntUniform("uHDRBuffer", 0);
	mScreenBufferShader->SetFloatUniform("exposure", 0.1);
	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

/// <summary>
/// HDRテクスチャとBloomテクスチャのブレンド
/// </summary>
void HDR::HDRBloomBlend()
{
	// レンダーターゲットをもとに戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_FRONT);

	glViewport(0, 0, mBufferWidth, mBufferHeght);

	// TEXTURE0にHDRテクスチャをバインド
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mColorBuffers[0]);

	// TEXTURE1,2,3,4,5にBloomテクスチャをバインド
	for (int i = 0; i < mMaxLevelNum; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, mBlurBufferTex[i * 2 + 1]);
	}

	mHDRBloomBlendShader->SetActive();
	mHDRBloomBlendShader->SetIntUniform("uScene", 0);
	mHDRBloomBlendShader->SetIntUniform("uBloom1", 1);
	mHDRBloomBlendShader->SetIntUniform("uBloom2", 2);
	mHDRBloomBlendShader->SetIntUniform("uBloom3", 3);
	mHDRBloomBlendShader->SetIntUniform("uBloom4", 4);
	mHDRBloomBlendShader->SetIntUniform("uBloom5", 5);

	const float exposure = 0.1;

	mHDRBloomBlendShader->SetFloatUniform("uExposure", exposure);

	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
