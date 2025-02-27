﻿#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Effekseer.h>
#include <EffekseerRendererGL.h>

#include "Math.h"
#include "VertexArray.h"
#include "ShaderTag.h"

// ディレクショナルライト構造体
typedef struct DirectionalLight
{
	//ライトの方向
	Vector3 mDirection;
	//拡散反射の色
	Vector3 mDiffuseColor;
	//鏡面反射の色
	Vector3 mSpecColor;
}DirectionalLight;

// 前方宣言
class SpriteComponent;

class Renderer
{
public:
	Renderer();
	~Renderer();
	bool                   Initialize(int screenWidth, int screenHeight, bool fullScreen);     // SDL & GL初期化
	void                   Shutdown();                                                         // 終了
	void                   Draw();                                                             // 描画

	// セッター系
	void                   SetViewMatrix(const Matrix4& view);                                 // ビュー行列のセット
	void                   SetProjMatrix(const Matrix4& proj);                                 // プロジェクション行列にセット
	void                   SetAmbientLight(const Vector3& ambientColor)                        // アンビエントライトのセット
	{
		mAmbientLight = ambientColor;
	}
	void SetDirectionalLight(const DirectionalLight& directionalLight) { mDirectionalLight = directionalLight; }
	void SetDepthSetting(const Vector3& centerPos, const Vector3& lightDir, const Vector3& upVec, const float lightDistance);
	void SetActiveSkyBox(class CubeMapComponent* cubeMapComp) { mSkyBox = cubeMapComp; }
	// ゲッター系
	SDL_Renderer* GetSDLRenderer() { return mSDLRenderer; }                           // SDL系の描画に必要なSDLrendererを得る
	class Texture* GetTexture(const std::string& fileName);                            // テクスチャをファイル名から返す
	class Mesh* GetMesh(const std::string& fileName);                               // メッシュをファイル名から返す
	const class Skeleton* GetSkeleton(const char* fileName);                                  // スケルタルモデルの取得
	const class Animation* GetAnimation(const char* fileName, bool loop);                      // スケルタルアニメーションの取得
	class EffekseerEffect* GetEffect(const char16_t* fileName);

	float                  GetScreenWidth() { return static_cast<float>(mScreenWidth); }       // スクリーン幅
	float                  GetScreenHeight() { return static_cast<float>(mScreenHeight); }     // スクリーン高さ
	DirectionalLight& GetDirectionalLight() { return mDirectionalLight; }                      // ディレクショナルライト
	const Matrix4& GetViewMatrix() { return mView; }
	const Matrix4& GetProjectionMatrix() { return mProjection; }
	VertexArray* GetCubeMapVerts() { return mCubeMapVerts; }                             // キューブマップで使用するVertexArrayのポインタ


	void                   AddMeshComponent(class MeshComponent* mesh, ShaderTag shaderTag);    // メッシュコンポーネントの追加
	void                   RemoveMeshComponent(class MeshComponent* mesh,ShaderTag shaderTag);  // メッシュコンポーネントの削除
	void                   AddSprite(SpriteComponent* Sprite);
	void                   RemoveSprite(SpriteComponent* sprite);
	void                   ShowResource();                                                     // 登録されている テクスチャ・メッシュリソースの表示（デバッグ用）
	void                   WindowClear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); } // ウィンドウ描画クリア
	void                   WindowFlip() { SDL_GL_SwapWindow(mWindow); }                        // ウィンドウフリップ
	void                   SetWindowTitle(const std::string& title);                           // ウィンドウタイトルのセット

	void                   SpriteDrawBegin();
	void                   SpriteDrawEnd();

	void                   DrawTexture(class Texture* texture,
		int index, int xDivNum, int yDivNum,
		const Vector2& offset,
		float scale = 1.0f, float alpha = 1.0f);
	void                   DrawTexture(class Texture* texture, const Vector2& offset,
		float scale = 1.0f, float alpha = 1.0f);

	void                   DrawHelthGaugeTexture(class Texture* texture, int index, int xDivNum, int yDivNum,
		const Vector2& offset, float scaleX, float scaleY, float alpha);
	void                   DrawHelthGauge(class Texture* texture, const Vector2& offset,
		float scaleX, float scaleY, float alpha = 1.0f);

	// Effekseer関連
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> GetEffekseerRenderer() { return mEffekseerRenderer; }
	Effekseer::RefPtr<Effekseer::Manager> GetEffekseerManager() { return mEffekseerManager; }

private:

	bool                                              LoadShaders();                          // シェーダーの初期化
	void                                              SetLightUniforms(class Shader* shader); // ライト値をシェーダーにセット
	void                                              CreateSpriteVerts();                    // スプライト頂点作成
	void                                              CreateHealthGaugeVerts();               // 体力ゲージ用の頂点作成
	void                                              CreateCubeMapVerts();                   // キューブマップの頂点作成
	void                                              CreateQuadVAO();                        // 四角形の頂点配列作成
	void                                              CreateLightFBO();                       // ライト結果書き込み用のフレームバッファの作成
	void                                              ScreenVAOSetting(unsigned int& vao);    // 画面全体を覆う頂点定義
	void                                              BakeDepthMap();                         // デプスマップに焼きこむ
	void                                              PointLightPass();                       // ポイントライトパス
	void                                              DirectionalLightPass();
	void                                              RenderQuad();                           // 四角形の描画
	void                                              DeferredRendering();                    // ディファードレンダリング
	void                                              CalcAttenuationLightRadius(const float constant,const float linear,const float quadratic);           // ライトの減衰半径の計算  

	int                                               mScreenWidth;       // スクリーン幅                                                           
	int                                               mScreenHeight;      // スクリーン高さ
	std::unordered_map<std::string, class Texture*>   mTextures;          // テクスチャ登録配列
	std::unordered_map<std::string, class Mesh*>      mMeshs;             // メッシュ登録配列
	std::vector<class MeshComponent*>                 mMeshComponents;    // メッシュコンポーネント登録配列
	std::vector<class MeshComponent*>                 mHighLightMeshes;   // HDRメッシュ
	std::vector<class MeshComponent*>                 mNoramlMeshes;
	std::vector<class SkeletalMeshComponent*>         mSkeletalMeshes;    // スケルタルメッシュの描画に使われる
	class CubeMapComponent* mSkyBox;
	std::unordered_map<std::string, class Skeleton*>  mSkeletons;         // スケルタルデータ
	std::unordered_map<std::string, class Animation*> mAnims;             // アニメーションデータ
	std::vector<SpriteComponent*>                     mSprites;           // スプライトの描画に使われるスプライトコンポーネントのポインタの可変長コンテナ
	std::unordered_map<const char16_t*, class EffekseerEffect*> mEffects; // エフェクト
	std::vector<Vector3> mLightPos;
	std::vector<Vector3> mLightColor;
	int mLightNum;

// シェーダー関連 //
	class Shader* mSpriteShader;           // スプライトシェーダー
	class Shader* mTilemapShader;          // タイルマップシェーダー
	class Shader* mMeshShader;             // メッシュシェーダー
	class Shader* mMeshDepthShader;        // メッシュのデプスシェーダー
	class Shader* mSkinnedShader;          // スキンメッシュシェーダー
	class Shader* mSkinnedDepthShader;     // スキンメッシュのデプスシェーダー
	class Shader* mShadowMapShader;        // シャドウマップシェーダー
	class Shader* mHDRShader;              // HDRシェーダー
	class Shader* mNormalShader;           // 法線マップシェーダー
	class Shader* mSkyBoxShader;           // スカイボックスシェーダー
	class Shader* mGBufferShader;          // G-Bufferシェーダー
	class Shader* mDeferredLightingShader; // ディファードライティングシェーダー
	class Shader* mLightingShader;
	class Shader* mPointLightShader;       // ポイントライトシェーダー
	class Shader* mDirectionalLightShader;
	class Shader* mHighLightShader;

	class DepthMap* mDepthMapRenderer;     // デプスレンダラー
	class HDR* mHDRRenderer;               // HDRレンダラー
	class GBuffer* mGBufferRenderer;       // GBufferレンダラー

// 基本行列関連 //
	Matrix4                                           mView;             // ビュー行列
	Matrix4                                           mProjection;       // プロジェクション行列
	Matrix4                                           mLightSpaceMat;

// スプライト頂点配列 //
	class VertexArray* mSpriteVerts;
	class VertexArray* mHealthVerts;
	class VertexArray* mCubeMapVerts;

	// 頂点配列オブジェクトID
	unsigned int mVertexArray;
	// 頂点バッファID
	unsigned int mVertexBuffer;
	// インデックスID
	unsigned int mIndexBuffer;

	// フレームバッファID
	unsigned int mLightFBO;
	unsigned int mLightHDRTex;

// ライティング関連 //
	Vector3                                           mAmbientLight;     // アンビエントライト
	DirectionalLight                                  mDirectionalLight; // ディレクショナルライト

	std::vector<float> lightRadius;// ライトの減衰半径

// レンダリングベース情報関連 //
	SDL_Window* mWindow;             // SDLウィンドウハンドル 
	SDL_GLContext mContext;          // OpenGLコンテキスト
	SDL_Renderer* mSDLRenderer;      // SDLレンダリングハンドル

	const char* mGlslVersion;      // GLSLのバージョン

// Effekseer関連 //
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> mEffekseerRenderer; // Effekseerレンダラ
	Effekseer::RefPtr<Effekseer::Manager>            mEffekseerManager; // Effekseerマネージャ  
};

bool GLErrorHandle(const char* location);                              // OpenGLのエラーハンドル取得