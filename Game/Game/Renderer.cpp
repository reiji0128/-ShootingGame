#include "Renderer.h"
#include "Game.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "MeshComponent.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "SpriteComponent.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "Effekseer.h"
#include "EffekseerEffect.h"
#include "DepthMap.h"
#include "HDR.h"
#include "CubeMapComponent.h"
#include "GBuffer.h"

Renderer::Renderer()
	:mWindow(nullptr)
	, mSDLRenderer(nullptr)
	, mContext(0)
	, mMeshShader(nullptr)
	, mSkinnedShader(nullptr)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

//	OpenGLの各属性を設定する //
	// コアOpenGLプロファイルを使う
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// GL version 3.4
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// 8Bit RGBA チャンネル
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// ダブルバッファを有効
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// ハードウェアアクセラレーションを使う
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Windowの作成
	mWindow = SDL_CreateWindow("SDL & GL Window",
		100, 80,
		mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!mWindow)
	{
		printf("Windowの作成に失敗: %s", SDL_GetError());
		return false;
	}
	if (fullScreen)
	{
		if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			printf("(%d, %d) サイズのフルスクリーン化に失敗\n", screenWidth, screenHeight);
			return false;
		}
		mScreenWidth = 1920;
		mScreenHeight = 1080;
		glViewport(0, 0, mScreenWidth, mScreenHeight);
	}

	//SDLRendererの作成
	mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mSDLRenderer)
	{
		printf("SDLRendererの作成に失敗 : %s", SDL_GetError());
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDLImageInitPNGの初期化に失敗 : %s", SDL_GetError());
		return false;
	}

	// OpenGLContextの作成
	mContext = SDL_GL_CreateContext(mWindow);

	// Glewの初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEWの初期化に失敗");
		return false;
	}
	// 幾つかのプラットホームでは、GLEWが無害なエラーコードを吐くのでクリアしておく
	glGetError();

	// シェーダーのロード
	if (!LoadShaders())
	{
		printf("シェーダーのロードに失敗");
		return false;
	}

	// デプスレンダラーの初期化
	mDepthMapRenderer = new DepthMap;
	mDepthMapRenderer->CreateShadowMap();

	// HDRの初期化
	mHDRRenderer = new HDR;
	mHDRRenderer->CreateHDRBuffer();

	// G-bufferレンダラーの初期化
	mGBufferRenderer = new GBuffer;
	mGBufferRenderer->CreateGbuffer();

	// カリング
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	
	// スプライト用の頂点配列を作成
	CreateSpriteVerts();
	// 体力ゲージ用の頂点配列を作成
	CreateHealthGaugeVerts();
	// キューブマップ用の頂点配列を作成
	CreateCubeMapVerts();
	// スクリーンを覆う四角形の頂点配列作成
	CreateQuadVAO();
	// ライト用のフレームバッファ作成
	CreateLightFBO();
	// スクリーン全体を覆う頂点バッファオブジェクトを作成
	unsigned int screenVAO;
	ScreenVAOSetting(screenVAO);

	// Effekseer初期化
	mEffekseerRenderer = ::EffekseerRendererGL::Renderer::Create(8000, EffekseerRendererGL::OpenGLDeviceType::OpenGL3);
	mEffekseerManager = ::Effekseer::Manager::Create(8000);

	// 描画モジュール作成
	mEffekseerManager->SetSpriteRenderer(mEffekseerRenderer->CreateSpriteRenderer());
	mEffekseerManager->SetRibbonRenderer(mEffekseerRenderer->CreateRibbonRenderer());
	mEffekseerManager->SetRingRenderer(mEffekseerRenderer->CreateRingRenderer());
	mEffekseerManager->SetTrackRenderer(mEffekseerRenderer->CreateTrackRenderer());
	mEffekseerManager->SetModelRenderer(mEffekseerRenderer->CreateModelRenderer());

	// Effekseer用のテクスチャ・モデル・マテリアルローダー
	mEffekseerManager->SetTextureLoader(mEffekseerRenderer->CreateTextureLoader());
	mEffekseerManager->SetModelLoader(mEffekseerRenderer->CreateModelLoader());
	mEffekseerManager->SetMaterialLoader(mEffekseerRenderer->CreateMaterialLoader());

	for (int i = 0; i < mLightNum; i++)
	{
		Vector3 pos((rand() % (2309 - -474 + 1)) + -474,
			        (rand() % (1069 - -1283 + 1)) + -1283,
			        (rand() % (40 - 30 + 1)) + 30);
		mLightPos.push_back(pos);

		Vector3 color(rand() % 100 / 100.0f,
			          rand() % 100 / 100.0f,
			          rand() % 100 / 100.0f);
		mLightColor.push_back(color);
	}

	return true;
}

void Renderer::Shutdown()
{
	//テクスチャ破棄
	for (auto i : mTextures)
	{
		printf("Texture Release : %s\n", i.first.c_str());
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// メッシュ破棄
	for (auto i : mMeshs)
	{
		printf("Mesh Release : %s\n", i.first.c_str());
		i.second->Unload();
		delete i.second;
	}
	mMeshs.clear();

	//シェーダー破棄
	mMeshShader->Unload();

	// スケルトンの破棄
	for (auto s : mSkeletons)
	{
		delete s.second;
	}

	// アニメーションの破棄
	for (auto a : mAnims)
	{
		delete a.second;
	}

	// エフェクトの破棄
	for (auto e : mEffects)
	{
		delete e.second;
	}

	delete mDepthMapRenderer;
	delete mHDRRenderer;
	delete mGBufferRenderer;

	// Effekseer関連の破棄
	mEffekseerManager.Reset();
	mEffekseerRenderer.Reset();

	// SDL系の破棄
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	DeferredRendering();

	


	/*mLightingShader->SetActive();
	mLightingShader->SetMatrixUniform("projection", mProjection);
	mLightingShader->SetMatrixUniform("view", mView);
	mLightingShader->SetMatrixUniform("uViewProj", mView * mProjection);
	for (int i = 0; i < mLightPos.size(); i++)
	{
		for (auto mc : mHighLightMeshes)
		{
			mLightingShader->SetVectorUniform("lightColor", mLightColor[i]);
			Matrix4 trans = Matrix4::CreateTranslation(mLightPos[i]);
			mLightingShader->SetMatrixUniform("model", trans);
			mc->Draw(mLightingShader);
		}
	}*/

	//Matrix4 lightSpaceMat = mDepthMapRenderer->GetLightSpaceMatrix();

	//// デプスレンダリングパス開始
	//mDepthMapRenderer->DepthRenderingBegin();
	//{
	//	// ライト空間行列を取得

	//	// スタティックメッシュをデプスへレンダリング
	//	mMeshDepthShader->SetActive();
	//	mMeshDepthShader->SetMatrixUniform("lightSpaceMatrix", lightSpaceMat);
	//	for (auto mc : mMeshComponents)
	//	{
	//		if (mc->GetVisible())
	//		{
	//			mc->Draw(mMeshDepthShader);
	//		}
	//	}

	//	// スキンメッシュをデプスへレンダリング
	//	mSkinnedDepthShader->SetActive();
	//	mSkinnedDepthShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
	//	for (auto sk : mSkeletalMeshes)
	//	{
	//		if (sk->GetVisible())
	//		{
	//			sk->Draw(mSkinnedDepthShader);
	//		}
	//	}
	//}
	//// デプスレンダリングの終了
	//mDepthMapRenderer->DepthRenderingEnd();

	//if (mSkyBox != nullptr)
	//{
	//	mSkyBoxShader->SetActive();
	//	// ゲームの空間に合わせるためのオフセット行列をセット
	//	Matrix4 offset = Matrix4::CreateRotationX(Math::ToRadians(90.0f));

	//	// Uniformに逆行列をセット
	//	Matrix4 InvView = mView;
	//	InvView.Invert();
	//	InvView.Transpose();
	//	mSkyBoxShader->SetMatrixUniform("uOffset", offset);
	//	mSkyBoxShader->SetMatrixUniform("uProjection", mProjection);
	//	mSkyBoxShader->SetMatrixUniform("uView", InvView);
	//	mSkyBoxShader->SetIntUniform("uSkyBox", 0);

	//	mSkyBox->Draw(mSkyBoxShader);
	//}

	//mNormalShader->SetActive();
	//mNormalShader->SetVectorUniform("in_light.position", mDirectionalLight.mDirection);
	//mNormalShader->SetVectorUniform("in_light.diffuse", mDirectionalLight.mDiffuseColor);
	//mNormalShader->SetVectorUniform("in_light.specular", mDirectionalLight.mSpecColor);
	//mNormalShader->SetVectorUniform("in_light.ambient", mAmbientLight);
	//mNormalShader->SetMatrixUniform("view", mView);
	//mNormalShader->SetMatrixUniform("projection", mProjection);
	//mNormalShader->SetVectorUniform("viewPos", GAMEINSTANCE.GetViewPos());
	//mNormalShader->SetIntUniform("normalMap", 1);
	//for (auto sk : mNoramlMeshes)
	//{
	//	if (sk->GetVisible())
	//	{
	//		sk->Draw(mNormalShader);
	//	}
	//}

	//mHDRRenderer->HDRRenderingBegin();
	//{
	//	

	//	// 通常レンダリング(シャドウ付き)
	//	mShadowMapShader->SetActive();
	//	// カメラの位置
	//	mShadowMapShader->SetVectorUniform("uCameraPos", GAMEINSTANCE.GetViewPos());
	//	//アンビエントライト
	//	mShadowMapShader->SetVectorUniform("uAmbientLight", mAmbientLight);
	//	//ディレクショナルライト
	//	mShadowMapShader->SetVectorUniform("uDirLight.mDirection", mDirectionalLight.mDirection);
	//	mShadowMapShader->SetVectorUniform("uDirLight.mDiffuseColor", mDirectionalLight.mDiffuseColor);
	//	mShadowMapShader->SetVectorUniform("uDirLight.mSpecColor", mDirectionalLight.mSpecColor);
	//	mShadowMapShader->SetMatrixUniform("uView", mView);
	//	mShadowMapShader->SetMatrixUniform("uProjection", mProjection);

	//	// デプスマップをセット（メッシュ用）
	//	glActiveTexture(GL_TEXTURE1);
	//	glBindTexture(GL_TEXTURE_2D, mDepthMapRenderer->GetDepthTexID());
	//	// 追加パラメーター
	//	mShadowMapShader->SetIntUniform("depthMap", 1);
	//	mShadowMapShader->SetMatrixUniform("uLightSpaceMatrix", lightSpaceMat);

	//	// 全てのメッシュコンポーネントを描画
	//	for (auto mc : mMeshComponents)
	//	{
	//		if (mc->GetVisible())
	//		{
	//			mc->Draw(mShadowMapShader);
	//		}
	//	}

	//	// mSkinnedShaderをアクティブ
	//	mSkinnedShader->SetActive();
	//	// カメラの位置
	//	mSkinnedShader->SetVectorUniform("uCameraPos", GAMEINSTANCE.GetViewPos());
	//	//アンビエントライト
	//	mSkinnedShader->SetVectorUniform("uAmbientLight", mAmbientLight);
	//	//ディレクショナルライト
	//	mSkinnedShader->SetVectorUniform("uDirLight.mDirection", mDirectionalLight.mDirection);
	//	mSkinnedShader->SetVectorUniform("uDirLight.mDiffuseColor", mDirectionalLight.mDiffuseColor);
	//	mSkinnedShader->SetVectorUniform("uDirLight.mSpecColor", mDirectionalLight.mSpecColor);
	//	mSkinnedShader->SetMatrixUniform("uView", mView);
	//	mSkinnedShader->SetMatrixUniform("uProjection", mProjection);

	//	// デプスマップをセット（メッシュ用）
	//	glActiveTexture(GL_TEXTURE1);
	//	glBindTexture(GL_TEXTURE_2D, mDepthMapRenderer->GetDepthTexID());
	//	// 追加パラメーター
	//	mSkinnedShader->SetIntUniform("depthMap", 1);
	//	mSkinnedShader->SetMatrixUniform("uLightSpaceMatrix", lightSpaceMat);
	//	// ビュー射影行列のセット
	//	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);
	//	// Uniform変数にライトをセット
	//	SetLightUniforms(mSkinnedShader);
	//	for (auto sk : mSkeletalMeshes)
	//	{
	//		if (sk->GetVisible())
	//		{
	//			sk->Draw(mSkinnedShader);
	//		}
	//	}

	//	//球体の描画
	//	Vector3 lightColor(0.8, 0.5, 0.2);
	//	mHDRShader->SetActive();
	//	mHDRShader->SetMatrixUniform("uViewProj", mView * mProjection);
	//	mHDRShader->SetVectorUniform("color", lightColor);
	//	mHDRShader->SetFloatUniform("luminance", 10.0f);
	//	// 全てのメッシュコンポーネントを描画
	//	for (auto mc : mHighLightMeshes)
	//	{
	//		if (mc->GetVisible())
	//		{
	//			mc->Draw(mHDRShader);
	//		}
	//	}
	//}
	//mHDRRenderer->HDRRenderingEnd();

	//// hdrカラーバッファを2Dスクリーンを埋め尽くす四角形ポリゴンに描画
	//// この時トーンマッピングを行ってHDR画像をLDRにする
	////mHDRRenderer->RenderQuad();
	//mHDRRenderer->ScaleDownBufferPath();
	//mHDRRenderer->HDRBloomBlend();

	GAMEINSTANCE.GetPhysics()->DebugShowBox();
}

void Renderer::SetViewMatrix(const Matrix4& view)
{
	Matrix4 tmp = view;
	mView = view;

	// Effekseer に座標系を合わせて行列をセットする
	Effekseer::Matrix44 efCam;
	tmp.mat[0][0] *= -1;
	tmp.mat[0][1] *= -1;
	tmp.mat[1][2] *= -1;
	tmp.mat[2][2] *= -1;
	tmp.mat[3][2] *= -1;

	efCam = tmp;
	RENDERER->GetEffekseerRenderer()->SetCameraMatrix(efCam);
}

void Renderer::SetProjMatrix(const Matrix4& proj)
{
	mProjection = proj;

	// Effekseer に座標系を合わせて行列をセットする
	Matrix4 tmp = proj;
	tmp.mat[2][2] *= -1;
	tmp.mat[2][3] *= -1;

	Effekseer::Matrix44 eProj;
	eProj = tmp;
	RENDERER->GetEffekseerRenderer()->SetProjectionMatrix(eProj);
}

void Renderer::SetDepthSetting(const Vector3& centerPos, const Vector3& lightDir, const Vector3& upVec, const float lightDistance)
{
	mDepthMapRenderer->CalcLightSpaceMatrix(centerPos, lightDir, upVec, lightDistance);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	printf(" Load Texture Success : %s \n", fileName.c_str());
	return tex;
}

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshs.find(fileName);

	if (iter != mMeshs.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh;
		if (m->Load(fileName, this))
		{
			mMeshs.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	printf(" Load Mesh Success : %s\n", fileName.c_str());
	return m;
}

void Renderer::AddMeshComponent(MeshComponent* mesh, ShaderTag shaderTag)
{

	if(shaderTag == ShaderTag::SkinnedDepthmapAndSkinnedShadowMap)
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		mSkeletalMeshes.emplace_back(sk);
	}
	else if(shaderTag == ShaderTag::DepthmapAndShadowMap)
	{
		mMeshComponents.emplace_back(mesh);
	}
	else if (shaderTag == ShaderTag::HDR)
	{
		mHighLightMeshes.emplace_back(mesh);
	}
	else if (shaderTag == ShaderTag::Normal)
	{
		mNoramlMeshes.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComponent(MeshComponent* mesh,ShaderTag shaderTag)
{
	if (shaderTag == ShaderTag::SkinnedDepthmapAndSkinnedShadowMap)

	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
		mSkeletalMeshes.erase(iter);
	}
	else if (shaderTag == ShaderTag::DepthmapAndShadowMap)
	{
		auto iter = std::find(mMeshComponents.begin(), mMeshComponents.end(), mesh);
		mMeshComponents.erase(iter);
	}
	else if (shaderTag == ShaderTag::HDR)
	{
		auto iter = std::find(mHighLightMeshes.begin(), mHighLightMeshes.end(), mesh);
		mHighLightMeshes.erase(iter);
	}
	else if (shaderTag == ShaderTag::Normal)
	{
		auto iter = std::find(mNoramlMeshes.begin(), mNoramlMeshes.end(), mesh);
		mNoramlMeshes.erase(iter);
	}

}

/// <summary>
/// スプライトの追加
/// </summary>
/// <param name="sprite">追加するSpriteComponentクラスのポインタ</param>
void Renderer::AddSprite(SpriteComponent* sprite)
{
	// ソート済みの配列で挿入点を見つける
	// (DrawOrderが小さい順番に描画するため)
	int myDrowOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();

	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrowOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// イテレーターの位置の前に要素を挿入する
	mSprites.insert(iter, sprite);
}

/// <summary>
/// スプライトの削除
/// </summary>
/// <param name="sprite">削除するSpriteComponentクラスのポインタ</param>
void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = 
	{
	    //    位置　　　 |　   法線       |  uv座標
		-0.5f, 0.5f, 0.f,  0.f, 0.f, 0.0f,  0.f, 0.f, // 左上
		 0.5f, 0.5f, 0.f,  0.f, 0.f, 0.0f,  1.f, 0.f, // 右上
		 0.5f,-0.5f, 0.f,  0.f, 0.f, 0.0f,  1.f, 1.f, // 右下
		-0.5f,-0.5f, 0.f,  0.f, 0.f, 0.0f,  0.f, 1.f  // 左下
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::CreateHealthGaugeVerts()
{
	float vertices[] =
	{
	    //    位置　　　 |　   法線       |  uv座標
		0.0f, 1.0f, 0.0f,  0.f, 0.f, 0.0f,  0.f, 0.f,	//左上  0
		1.0f, 1.0f, 0.0f,  0.f, 0.f, 0.0f,  1.f, 0.f,	//右上  1
		0.0f, 0.0f, 0.0f,  0.f, 0.f, 0.0f,  1.f, 1.f,	//左下  2
		1.0f, 0.0f, 0.0f,  0.f, 0.f, 0.0f,  0.f, 1.f,	//右下  3
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 1
	};

	mHealthVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::CreateCubeMapVerts()
{
	mCubeMapVerts = new VertexArray();
	mCubeMapVerts->CreateCubeMapVAO();
}

void Renderer::CreateQuadVAO()
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

void Renderer::CreateLightFBO()
{
	unsigned int lightHDRTex, lightRBO;

	glGenFramebuffers(1, &mLightFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, mLightFBO);

	// HDRテクスチャの作成
	glGenTextures(1, &lightHDRTex);
	glBindTexture(GL_TEXTURE_2D, lightHDRTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mScreenWidth, mScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,lightHDRTex,0);

	// OpenGLにカラーテクスチャアタッチメント0を使用することを伝える
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// レンダーバッファを作成
	glGenRenderbuffers(1, &lightRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, lightRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mScreenWidth, mScreenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, lightRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "LightBuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::ScreenVAOSetting(unsigned int& vao)
{
	unsigned int vbo;
	float quadVertices[] =
	{
		//      位置       |   uv座標
		-1.0f,  1.0f, 0.0f,  0.0f, 0.0f,   // 左上
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // 左下
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // 右上
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f    // 右下
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Renderer::PointLightPass()
{
	MeshComponent* sphereMesh = mHighLightMeshes[0];

	// ビュー行列の設定
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f),
		static_cast<float>(mScreenWidth),
		static_cast<float>(mScreenHeight),
		1.0f, 10000.0f);

	// 深度テストを無効
	glDisable(GL_DEPTH_TEST);

	// ライトボリュームの裏側だけ描画するように設定
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);


	const float constant = 1.0f;   // 定数
	const float linear = 0.7f;     // 線形
	const float quadratic = 1.8f;   //2乗項

	// ポイントライトシェーダーにパラメーターをセット
	Vector3 lightSpecular = Vector3(1.0f, 1.0f, 1.0f);
	mPointLightShader->SetActive();
	mPointLightShader->SetFloatUniform("uLight.constant", constant);
	mPointLightShader->SetFloatUniform("uLight.linear", linear);
	mPointLightShader->SetFloatUniform("uLight.quadratic", quadratic);
	mPointLightShader->SetVectorUniform("uViewPos", GAMEINSTANCE.GetViewPos());
	mPointLightShader->SetVectorUniform("uLight.specular", lightSpecular);
	mPointLightShader->SetMatrixUniform("uView", mView);
	mPointLightShader->SetMatrixUniform("uProjection", mProjection);
	mPointLightShader->SetIntUniform("gPosition", 0);
	mPointLightShader->SetIntUniform("gNormal", 1);
	mPointLightShader->SetIntUniform("gAlbedoSpec", 2);

	CalcAttenuationLightRadius(constant, linear, quadratic);

	// 個数分のライト描画
	for (unsigned int i = 0; i < mLightPos.size(); i++)
	{
		Vector3 ambient = mLightColor[i] * 0.2f;
		mPointLightShader->SetVectorUniform("uLight.position", mLightPos[i]);
		mPointLightShader->SetVectorUniform("uLight.diffuse", mLightColor[i]);
		mPointLightShader->SetVectorUniform("uLight.ambient", ambient);

		Matrix4 mat = Matrix4::CreateScale(lightRadius[i]);
		mat = mat * Matrix4::CreateTranslation(mLightPos[i]);
		mPointLightShader->SetMatrixUniform("model", mat);

		sphereMesh->Draw(mPointLightShader, false);
	}

	// 裏側描画OFF
	glDisable(GL_CULL_FACE);
}

void Renderer::DirectionalLightPass()
{
	glDisable(GL_DEPTH_TEST);

	// ライトカラー設定
	Vector3 ambientColor, color, specular;
	ambientColor = Vector3(0.1f, 0.1f, 0.1f);
	color = Vector3(0.3f, 0.3f, 0.3f);
	specular = Vector3(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;

	mDirectionalLightShader->SetActive();
	mDirectionalLightShader->SetVectorUniform("uViewPos"            , GAMEINSTANCE.GetViewPos());
	mDirectionalLightShader->SetVectorUniform("uLight.direction"    , mDirectionalLight.mDirection);
	mDirectionalLightShader->SetVectorUniform("uLight.ambientColor" , ambientColor);
	mDirectionalLightShader->SetVectorUniform("uLight.color"        , color);
	mDirectionalLightShader->SetVectorUniform("uLight.specular"     , specular);
	mDirectionalLightShader->SetFloatUniform("uLight.intensity"     , intensity);

	// テクスチャをシェーダーにセット
	mGBufferRenderer->InputGBufferToShader();

	mDirectionalLightShader->SetIntUniform("gPosition"   , 0);
	mDirectionalLightShader->SetIntUniform("gNormal"     , 1);
	mDirectionalLightShader->SetIntUniform("gAlbedoSpec" , 2);


	// スクリーンいっぱいの四角形を描画
	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::RenderQuad()
{
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	if (quadVAO == 0)
	{
		float quadVertices[] =
		{
			//      座標      //  uv座標
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
}

void Renderer::DeferredRendering()
{
	// G-Bufferにメッシュとスケルタルメッシュを描画
	mGBufferRenderer->GBufferRenderingBegin();
	{
		mGBufferShader->SetActive();
		mGBufferShader->SetMatrixUniform("uViewProj", mView * mProjection);
		for (auto mc : mMeshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(mGBufferShader);
			}
		}
	}
	mGBufferRenderer->GBufferRenderingEnd();

	// ライティングパス開始
	glBindFramebuffer(GL_FRAMEBUFFER, mLightFBO);
	{
		// 加算合成を有効
		glEnablei(GL_BLEND, 0);
		glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// G-Bufferテクスチャをシェーダーに渡す
		mGBufferRenderer->InputGBufferToShader();

		PointLightPass();

		DirectionalLightPass();

		// 加算合成を無効
		glDisablei(GL_BLEND, 0);
	}
	// ライティングパス終了
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic)
{
	
	// 解の公式より減衰半径を計算
	for (unsigned int i = 0; i < mLightColor.size(); i++)
	{
		Vector3 color = mLightColor[i];
		float max = std::fmax(std::fmax(color.x, color.y), color.z);
		float radius = (-linear + std::sqrtf((linear * linear) - 4.0f * 
			            quadratic * (constant - (256.0f / 4.0f) * max))) / (2.0f * quadratic);

		lightRadius.push_back(radius);
	}

}


void Renderer::ShowResource()
{
	printf("\n\n<------------------ textures ------------------>\n");
	for (auto i : mTextures)
	{
		printf("texfile %s\n", i.first.c_str());
	}

	printf("\n<------------------  meshes  ------------------->\n");
	for (auto i : mMeshs)
	{
		printf("meshfile %s\n", i.first.c_str());
	}
}

void Renderer::SetWindowTitle(const std::string& title)
{
	SDL_SetWindowTitle(mWindow, title.c_str());
}

void Renderer::SpriteDrawBegin()
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	mTilemapShader->SetActive();
	mSpriteVerts->SetActive();
}

void Renderer::SpriteDrawEnd()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

// テクスチャの描画
void Renderer::DrawTexture(class Texture* texture, int index, int xDivNum, int yDivNum, const Vector2& offset, float scale, float alpha)
{
	// テクスチャの幅・高さでスケーリング
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth() / xDivNum) * scale,
		static_cast<float>(texture->GetHeight() / yDivNum) * scale,
		1.0f);
	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x - (mScreenWidth * 0.5f),
			(mScreenHeight * 0.5f) - offset.y, 0.0f));
	// ワールド変換
	Vector2 tileSplitNum(static_cast<float>(xDivNum), static_cast<float>(yDivNum));
	Matrix4 world = scaleMat * transMat;
	mTilemapShader->SetMatrixUniform("uWorldTransform", world);
	mTilemapShader->SetIntUniform("uTileIndex", index);
	mTilemapShader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	mTilemapShader->SetFloatUniform("uAlpha", alpha);
	// テクスチャセット
	texture->SetActive();

	// 四角形描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawTexture(Texture* texture, const Vector2& offset, float scale, float alpha)
{
	DrawTexture(texture, 0, 1, 1, offset, scale, alpha);
}

// 体力ゲージの描画
void Renderer::DrawHelthGaugeTexture(Texture* texture, int index, int xDivNum, int yDivNum, const Vector2& offset, float scaleX, float scaleY, float alpha)
{
	mHealthVerts->SetActive();
	// テクスチャの幅・高さでスケーリング
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texture->GetWidth() / xDivNum) * scaleX,
		static_cast<float>(texture->GetHeight() / yDivNum) * scaleY,
		1.0f);

	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x - (mScreenWidth * 0.5f),
		(mScreenHeight * 0.5f) - offset.y,
		0.0f));
	// ワールド変換
	Vector2 tileSplitNum(static_cast<float>(xDivNum), static_cast<float>(yDivNum));
	Matrix4 world = scaleMat * transMat;
	mTilemapShader->SetMatrixUniform("uWorldTransform", world);
	mTilemapShader->SetIntUniform("uTileIndex", index);
	mTilemapShader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	mTilemapShader->SetFloatUniform("uAlpha", alpha);
	// テクスチャセット
	texture->SetActive();

	// 四角形描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}


void Renderer::DrawHelthGauge(Texture* texture, const Vector2& offset, float scaleX, float scaleY, float alpha)
{
	DrawHelthGaugeTexture(texture, 0, 1, 1, offset, scaleX, scaleY, alpha);
}

bool Renderer::LoadShaders()
{
	// スプライトシェーダーのロード//
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		printf("スプライトシェーダーの読み込み失敗\n");
		return false;
	}
	mSpriteShader->SetActive();

	// ビュープロジェクション行列のセット
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(static_cast<float>(mScreenWidth), static_cast<float>(mScreenHeight));
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// タイルマップシェーダーのロード //
	mTilemapShader = new Shader();
	if (!mTilemapShader->Load("Shaders/Sprite.vert", "Shaders/Tilemap.frag"))
	{
		printf("タイルマップシェーダーの読み込み失敗\n");
		return false;
	}
	mTilemapShader->SetActive();

	// ビュープロジェクション行列のセット
	mTilemapShader->SetMatrixUniform("uViewProj", viewProj);

	// ビュー行列の設定
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		static_cast<float>(mScreenWidth),
		static_cast<float>(mScreenHeight),
		1.0f, 10000.0f);

	// メッシュシェーダーのロード //
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		printf("メッシュシェーダー読み込み失敗\n");
		return false;
	}
	mMeshShader->SetActive();
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// メッシュのデプスシェーダーのロード
	mMeshDepthShader = new Shader();
	if (!mMeshDepthShader->Load("Shaders/DepthMap.vert", "Shaders/DepthMap.frag"))
	{
		printf("メッシュのデプスシェーダーの読み込み失敗\n");
		return false;
	}
	mMeshDepthShader->SetActive();

	// スキンメッシュシェーダーのロード
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/SkinnedShadowmap.vert", "Shaders/Shadowmap.frag"))
	{
		printf("スキンメッシュシェーダの読み込み失敗\n");
		return false;
	}
	mSkinnedShader->SetActive();
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// スキンメッシュのデプスシェーダーのロード
	mSkinnedDepthShader = new Shader();
	if (!mSkinnedDepthShader->Load("Shaders/SkinnedDepthMap.vert", "Shaders/DepthMap.frag"))
	{
		printf("スキンメッシュのデプスシェーダーの読み込み失敗\n");
		return false;
	}
	mSkinnedDepthShader->SetActive();

	// シャドウマップシェーダーのロード
	mShadowMapShader = new Shader();
	if (!mShadowMapShader->Load("Shaders/Shadowmap.vert", "Shaders/Shadowmap.frag"))
	{
		printf("シャドウマップシェーダーの読み込み失敗\n");
		return false;
	}

	// スフィアシェーダーのロード
	mHDRShader = new Shader();
	if (!mHDRShader->Load("Shaders/Sphere.vert", "Shaders/Sphere.frag"))
	{
		return false;
	}

	// 法線マップシェーダーのロード
	mNormalShader = new Shader();
	if (!mNormalShader->Load("Shaders/Normal.vert", "Shaders/Normal.frag"))
	{
		return false;
	}

	// スカイボックスシェーダーのロード
	mSkyBoxShader = new Shader();
	if (!mSkyBoxShader->Load("Shaders/SkyBox.vert", "Shaders/SkyBox.frag"))
	{
		printf("スカイボックスシェーダーの読み込み失敗\n");
		return false;
	}

	// GBufferスシェーダーのロード
	mGBufferShader = new Shader();
	if (!mGBufferShader->Load("Shaders/gBuffer.vert", "Shaders/gBuffer.frag"))
	{
		printf("gBufferシェーダーの読み込み失敗\n");
		return false;
	}

	// ディファードライティングシェーダーのロード
	mDeferredLightingShader = new Shader();
	if (!mDeferredLightingShader->Load("Shaders/gBufferLightingPass.vert", "Shaders/gBufferLightingPass.frag"))
	{
		printf("ディファードライティングシェーダー読み込み失敗\n");
		return false;
	}

	// ライティングシェーダーのロード
	mLightingShader = new Shader();
	if (!mLightingShader->Load("Shaders/LightingSphere.vert", "Shaders/LightingSphere.frag"))
	{
		printf("ライティングシェーダー読み込み失敗\n");
		return false;
	}

	// ポイントライトシェーダーのロード
	mPointLightShader = new Shader();
	if (!mPointLightShader->Load("Shaders/PointLight.vert", "Shaders/PointLight.frag"))
	{
		printf("ポイントライトシェーダー読み込み失敗\n");
		return false;
	}

	// ディファードライティングシェーダー
	mDirectionalLightShader = new Shader();
	if (!mDirectionalLightShader->Load("Shaders/ScreenBuffer.vert", "Shaders/DirectionalLight.frag"))
	{
		printf("ディレクショナルライトシェーダー読み込み失敗\n");
		return false;
	}

	return true;
}

void Renderer::SetLightUniforms(Shader* shader)
{
	// ビュー行列からカメラ位置を逆算出する
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	//アンビエントライト
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	//ディレクショナルライト
	shader->SetVectorUniform("uDirLight.mDirection", mDirectionalLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirectionalLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirectionalLight.mSpecColor);
}

//////////////////////////////////////////////////////////////
// スケルタル情報の取得
// in  : スケルタル情報 .gpskelファイル名
// out : Skeleton情報へのポインタ
// Desc: gpskelファイル名より、スケルタル情報を返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpskelファイル名をキーとするスケルタル情報のmapが作成される
//////////////////////////////////////////////////////////////
const Skeleton* Renderer::GetSkeleton(const char* fileName)
{
	std::string file(fileName);
	auto iter = mSkeletons.find(file);
	if (iter != mSkeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(file))
		{
			mSkeletons.emplace(file, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

//////////////////////////////////////////////////////////////
// アニメーション情報の取得
// in  : アニメーションデータが格納されている .gpanimファイル名
// out : アニメーション情報へのポインタ
// Desc: gpanimファイル名よりアニメーションデータを返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpanimファイル名をキーとするアニメーション情報のmapが作成される
//////////////////////////////////////////////////////////////
const Animation* Renderer::GetAnimation(const char* fileName, bool loop)
{
	auto iter = mAnims.find(fileName);
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(fileName, loop))
		{
			mAnims.emplace(fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}

EffekseerEffect* Renderer::GetEffect(const char16_t* fileName)
{
	// エフェクトファイルを一度読み込んだかを確認
	EffekseerEffect* effect = nullptr;
	auto iter = mEffects.find(fileName);

	// 一度読んでいるなら返す
	if (iter != mEffects.end())
	{
		return iter->second;
	}
	//読んでいなければ新規追加
	effect = new EffekseerEffect;
	if (effect->LoadEffect(fileName))
	{
		mEffects.emplace(fileName, effect);
	}
	else
	{
		delete effect;
		effect = nullptr;
	}
	return effect;
}

bool GLErrorHandle(const char* location)
{
	GLenum error_code = glGetError();
	if (error_code == GL_NO_ERROR)
	{
		return true;
	}
	do
	{
		const char* msg;
		switch (error_code)
		{
		case GL_INVALID_ENUM:                  msg = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 msg = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             msg = "INVALID_OPERATION"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: msg = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: msg = "unknown Error";
		}
		printf("GLErrorLayer: ERROR%04x'%s' location: %s\n", error_code, msg, location);
		error_code = glGetError();
	} while (error_code != GL_NO_ERROR);

	return false;
}