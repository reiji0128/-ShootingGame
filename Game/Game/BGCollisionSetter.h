#pragma once

#include "Actor.h"

class BGCollisionSetter : public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="collisionBoxJsonFileName">BGXollisoinのファイルパス</param>
	BGCollisionSetter(const char* collisionBoxJsonFileName);

	/// <summary>
	//  デストラクタ
	/// </summary>
	~BGCollisionSetter();

private:
	/// <summary>
	/// Jsonファイルのロード
	/// </summary>
	/// <param name="collisionBoxJsonFileName">BGXollisoinのファイルパス</param>
	/// <returns>
	/// true  : 読み込み成功
	/// false : 読み込み失敗
	/// </returns>
	bool JsonFileLoad(const char* collisionBoxJsonFileName);

};