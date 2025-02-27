#pragma once
#include "Component.h"
#include <string>

class Shader;
class Texture;

class CubeMapComponent : public Component
{
public:
	CubeMapComponent(Actor* owner);
	~CubeMapComponent();

	void CreateTexture(const std::string& fileName);

	void Draw(Shader* shader);

// セッター　//

	/// <summary>
	/// 表示フラグのセット
	/// </summary>
	/// <param name="visible">
	/// true  : 表示する
	/// false : 表示しない
	/// </param>
	void SetVisible(bool visible) { mVisible = visible;}

// ゲッター //

	/// <summary>
	/// 表示フラグの取得
	/// </summary>
	/// <returns>
	/// true  : 表示する
	/// false : 表示しない
	/// </returns>
	bool GetVisible() const { return mVisible; }

private:
	// 表示フラグ
	bool mVisible;

	Texture* mTexture;
};