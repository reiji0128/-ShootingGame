#pragma once

class VertexArray
{
public:
	// 頂点レイアウト列挙
	enum Layout
	{
		// 位置&法線&テクスチャUV を持ったフォーマット
		PosNormTex, 

		// 位置&法線& "スキン用の影響ボーン＆重み情報" & テクスチャUV 
		PosNormSkinTex  
	};

	VertexArray(const void* verts, unsigned int numVerts, Layout layout,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	void SetActive();                                                     // この頂点配列をアクティブにして描画で使用する
	unsigned int GetNumIndices() const { return mNumIndices; }            // インデックス数を取得する
	unsigned int GetNumVerts() const { return mNumVerts; }                // 頂点数を取得する
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