#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

class GameClearScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameClearScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameClearScene();

	/// <summary>
	/// シーンの描画
	/// </summary>
	/// <returns></returns>
	SceneBase* Update();

	void Draw();

private:
	// ビットマップテキストのポインタ
	class BitmapText* mFont;

	// テクスチャクラスのポインタ
	class Texture* mGameClearTex;
};