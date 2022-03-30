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

	/// <summary>
	/// ビットマップテキストのポインタ
	/// </summary>
	class BitmapText* mFont;
};