#pragma once

#include "Component.h"
#include <cstddef>
#include "ShaderTag.h"

class MeshComponent :public Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">Actorのポインタ</param>
	/// <param name="shaderTag">適用するシェーダーのタグ</param>
	/// <param name="isSkeletal">スケルタルモデルかどうか?</param>
	MeshComponent(class Actor* owner, ShaderTag shaderTag, bool isSkeletal = false);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MeshComponent();

	/// <summary>
	/// メッシュコンポーネントの描画処理
	/// </summary>
	/// <param name="owner">Shaderのポインタ</param>
	virtual void Draw(class Shader* shader);

	void Draw(class Shader* shader, bool useTexture);

// セッター //
	/// <summary>
	/// メッシュのセット
	/// </summary>
	/// <param name="owner">Meshのポインタ</param>
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }

	/// <summary>
	/// テクスチャのインデックスのセット
	/// </summary>
	/// <param name="index">インデックス</param>
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

	/// <summary>
	/// 表示フラグのセット
	/// </summary>
	/// <param name="visible"></param>
	void SetVisible(bool visible) { mVisible = visible; }

// ゲッター //
	/// <summary>
	/// 表示フラグの取得
	/// </summary>
	/// <returns></returns>
	bool GetVisible() const { return mVisible; }

	/// <summary>
	/// スケルタルモデルかどうかの取得
	/// </summary>
	/// <returns></returns>
	bool GetIsSkeletal() const { return mIsSkeletal; }

protected:
	// Meshクラスのポインタ
	class Mesh* mMesh;

	// テクスチャのインデックス
	size_t mTextureIndex;

	// 法線マップのインデックス
	size_t mNormalMapIndex;

	// 表示フラグ
	bool mVisible;

	// スケルタルモデルかどうかのフラグ
	bool mIsSkeletal;

	ShaderTag mShaderTag;
};
