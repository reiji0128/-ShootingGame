#include <string>

class Texture
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Texture();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Texture();

	/// <summary>
	/// テクスチャをファイルからロード
	/// </summary>
	/// <param name="fileName">ファイルパス</param>
	/// <returns>
	/// true  : 読み込み成功
	/// false : 読み込み失敗
	/// </returns>
	bool Load(const std::string& fileName);

	/// <summary>
	/// キューブマップのロード
	/// </summary>
	/// <param name="fileName">キューブマップのファイルパス</param>
	/// <returns>
	/// true  : 読み込み成功
	/// false : 読み込み失敗
	/// </returns>
	bool LoadCubeMap(const std::string& fileName);

	/// <summary>
	/// テクスチャ解放処理
	/// </summary>
	void Unload();

	/// <summary>
	/// SDLサーフェスからテクスチャを作成する
	/// </summary>
	/// <param name="surface">SDL_Surfaceのポインタ</param>
	void CreateFromSurface(struct SDL_Surface* surface);

	/// <summary>
	/// レンダリングするテクスチャを作成
	/// </summary>
	/// <param name="width">テクスチャの幅</param>
	/// <param name="height">テクスチャの高さ</param>
	/// <param name="format">フォーマット</param>
	void CreateForRendering(int width, int height, unsigned int format);

// セッター //
	/// <summary>
	/// テクスチャをアクティブ(ポリゴン描画で使用）にする
	/// </summary>
	void SetActive();

	void SetActiveNormalMap();

// ゲッター //
	/// <summary>
	/// テクスチャIDの取得
	/// </summary>
	/// <returns>テクスチャID</returns>
	int GetTextureID()const { return mTextureID; }

	/// <summary>
	/// テクスチャの幅を取得
	/// </summary>
	/// <returns>テクスチャの幅</returns>
	int GetWidth() const { return mWidth; }

	/// <summary>
	/// テクスチャ高さを取得
	/// </summary>
	/// <returns>テクスチャ高さ</returns>
	int GetHeight() const { return mHeight; }


private:
	// テクスチャID
	unsigned int mTextureID;

	// テクスチャ幅
	int mWidth;

	// テクスチャ高さ
	int mHeight;
};