#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

class GameScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// シーンの更新処理
	/// </summary>
	/// <returns></returns>
	SceneBase* Update();

	/// <summary>
	/// シーンの描画処理
	/// </summary>
	void	   Draw();

private:
	float mLightDistance;

	float mHealthScaleX;

	class PlayerActor* player;

	/// ビットマップテキストのポインタ
	class BitmapText* mFont;

	/// テクスチャクラスのポインタ
	class Texture* mTex;

	/// デバッググリッドクラスのポインタ
	class DebugGrid* mGrid;

	//class Texture* mUI;

	class Altar* mAltar;
};