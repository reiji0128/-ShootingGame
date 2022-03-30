#pragma once

#include "Game.h"

//シーンベースクラス
class SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneBase() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneBase() {};

	/// <summary>
	/// シーンの更新処理
	/// </summary>
	/// <returns></returns>
	virtual SceneBase* Update() = 0;

	/// <summary>
	/// シーンの描画処理
	/// </summary>
	virtual void Draw() = 0;
};