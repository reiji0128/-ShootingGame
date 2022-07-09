#pragma once
#include "Math.h"

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
		PosNormTangentTex,
	};

	VertexArray(const void* verts, unsigned int numVerts, Layout layout,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	void CreateCubeMapVAO();

	/// <summary>
	/// タンジェントベクトルを計算
	/// </summary>
	/// <param name="destTangent">tangentを格納する変数</param>
	/// <param name="pos1">三角形面の頂点</param>
	/// <param name="pos2">三角形面の頂点</param>
	/// <param name="pos3">三角形面の頂点</param>
	/// <param name="uv1">三角形面の頂点に対応するUV座標</param>
	/// <param name="uv2">三角形面の頂点に対応するUV座標</param>
	/// <param name="uv3">三角形面の頂点に対応するUV座標</param>
	void CalcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3,
		const Vector2& uv1, const Vector2& uv2, const Vector2& uv3);

// セッター　//	

	/// <summary>
	/// 頂点配列をアクティイブにセット(描画で使用できるように)
	/// </summary>
	void SetActive();

	/// <summary>
	/// キューブマップの頂点配列をアクティブにセット(描画で使用できるように)
	/// </summary>
	void SetActiveCubeMap();

	/// <summary>
	/// 頂点配列にtangent(接法線)を書き込む
	/// </summary>
	/// <param name="desstfv">頂点 & UV & Tangent配列の先頭アドレス</param>
	/// <param name="index">頂点 & UV & Tangent配列の書き込み先インデックス</param>
	/// <param name="tangent">頂点 & UV & Tangent配列に書き込みたいtangent値</param>
	void SetTangent(float* destfv, int index, const Vector3& tangent);

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

	/// <summary>
	/// 頂点配列から頂点座標ベクトルだけを取り出す
	/// </summary>
	/// <param name="destPos">頂点の座標を格納する変数</param>
	/// <param name="fv">頂点 & UVデータ配列の先頭アドレスが格納されたポインタ</param>
	/// <param name="index">頂点 & UVデータ配列のインデックス値</param>
	void GetPosVec(Vector3& destPos, const float* fv, int index);

	/// <summary>
	/// 頂点配列から頂点座標ベクトルだけを取り出す
	/// </summary>
	/// <param name="destUV">頂点のUV座標を格納する変数</param>
	/// <param name="fv">頂点 & UVデータ配列の先頭アドレスが格納されたポインタ</param>
	/// <param name="index">頂点 & UVデータ配列のインデックス値</param>
	void GetUVVec(Vector2& destUV, const float* fv, int index);

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

	// キューブマップ用頂点配列オブジェクトID
	unsigned int mCubeMapVertexArray;
};