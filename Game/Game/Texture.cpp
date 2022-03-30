#include "Texture.h"
#include "Game.h"
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>

/// <summary>
/// コンストラクタ
/// </summary>
Texture::Texture()
	:mTextureID(0)
	,mWidth(0)
	,mHeight(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Texture::~Texture()
{

}

/// <summary>
/// テクスチャをファイルからロード
/// </summary>
/// <param name="fileName">ファイルパス</param>
/// <returns>
/// true  : 読み込み成功
/// false : 読み込み失敗
/// </returns>
bool Texture::Load(const std::string& fileName)
{
	int channels = 0;

	// SDLサーフェスをテクスチャから作成
	SDL_Texture* tex = nullptr;
	SDL_Surface* surf = IMG_Load(fileName.c_str());
	if (!surf)
	{
		printf("テクスチャ読み込みに失敗 %s", fileName.c_str());
		return false;
	}

	// サーフェスからテクスチャを作る
	tex = SDL_CreateTextureFromSurface(GAMEINSTANCE.GetSDLRenderer(), surf);
	if (!tex)
	{
		printf("サーフェスからテクスチャの作成に失敗 : %s", fileName.c_str());
		return false;
	}

	// 画像の幅、高さを取得
	mWidth = surf->w;
	mHeight = surf->h;
	channels = surf->format->BytesPerPixel;

	// OpenGLにテクスチャ登録
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
		GL_UNSIGNED_BYTE, surf->pixels);

	// SDLサーフェスは用済みなので解放
	SDL_FreeSurface(surf);

	// テクスチャフィルタリング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

/// <summary>
/// テクスチャ解放処理
/// </summary>
void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

/// <summary>
/// SDLサーフェスからテクスチャを作成する
/// </summary>
/// <param name="surface">SDL_Surfaceのポインタ</param>
void Texture::CreateFromSurface(SDL_Surface* surface)
{
	mWidth = surface->w;
	mHeight = surface->h;

	// GLテクスチャを生成
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, surface->pixels);

	//リニアフィルタリングを有効にする
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/// <summary>
/// レンダリングするテクスチャを作成
/// </summary>
/// <param name="width">テクスチャの幅</param>
/// <param name="height">テクスチャの高さ</param>
/// <param name="format">フォーマット</param>
void Texture::CreateForRendering(int width, int height, unsigned int format)
{
	mWidth = width;
	mHeight = height;

	//テクスチャIDの作成
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	//nullの初期データで画像の幅/高さを設定
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0,
		GL_RGB, GL_FLOAT, nullptr);

	//レンダリングするテクスチャに最近傍を使用
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

/// <summary>
/// テクスチャをアクティブ(ポリゴン描画で使用）にする
/// </summary>
void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}