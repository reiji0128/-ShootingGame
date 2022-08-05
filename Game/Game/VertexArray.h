#pragma once
#include "Math.h"
#include <vector>

class VertexArray
{
public:
	// 頂点レイアウト列挙
	enum Layout
	{
		// 位置&法線&テクスチャUV を持ったフォーマット
		PosNormTex, 

		// 位置&法線& "スキン用の影響ボーン＆重み情報" & テクスチャUV 
		PosNormSkinTex,  

		// 位置 & 法線 & テクスチャUV & 従法線
		PosNormTexTangent,
	};

	VertexArray();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="verts">頂点バッファの配列の先頭へのポインタ</param>
	/// <param name="numVerts">頂点数</param>
	/// <param name="layout">適用する頂点レイアウト</param>
	/// <param name="indices">インデックスバッファの配列の先頭へのポインタ</param>
	/// <param name="numIndices">インデックス数</param>
	VertexArray(const void* verts, unsigned int numVerts, Layout layout,
		const unsigned int* indices, unsigned int numIndices);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~VertexArray();

	void CreateCubeMapVAO();

// セッター　//	

	/// <summary>
	/// 頂点配列をアクティイブにセット(描画で使用できるように)
	/// </summary>
	void SetActive();

// ゲッター //

	/// <summary>
	/// インデックス数の取得
	/// </summary>
	/// <returns>インデックス数</returns>
	unsigned int GetNumIndices() const { return mNumIndices; }

	/// <summary>
	/// 頂点数の取得
	/// </summary>
	/// <returns>頂点数</returns>
	unsigned int GetNumVerts() const { return mNumVerts; }

private:
	// 頂点数
	unsigned int mNumVerts;

	// インデックス数 （ポリゴン面数×３)
	unsigned int mNumIndices;

	// 頂点バッファID（OpenGLに登録時に付与される）
	unsigned int mVertexBuffer;

	// インデックスバッファID (OpenGL登録時に付与）
	unsigned int mIndexBuffer;

	// 頂点配列オブジェクトID
	unsigned int mVertexArray;
};