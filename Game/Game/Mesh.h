#pragma once
#include <vector>
#include <string>
#include "Collision.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	//メッシュのロード・アンロード
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();

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

// セッター //
	void SetUseNormalMap(bool useNormalMap) { mUseNormalMap = useNormalMap; }

// ゲッター //
	//このメッシュに関連付けられている頂点配列を取得
	class VertexArray* GetVertexArray() { return mVertexArray; }
	//指定されたインデックスからテクスチャを取得
	class Texture* GetTexture(size_t intdex);
	//シェーダーの名前を取得
	const std::string& GetShaderName() const { return mShaderName; }

	const AABB& GetCollisionBox() const { return mBox; }

	//オブジェクト空間での境界球の半径を取得
	float GetRadius() const { return mRadius; }
	// スペキュラーの取得
	float GetSpecPower() const{ return mSpecPower; }

	bool GetUseNormalMap() const { return mUseNormalMap; }

private:
	//このメッシュに関連付けられているテクスチャ
	std::vector<class Texture*> mTextures;
	//このメッシュに関連付けられた頂点配列
	class VertexArray* mVertexArray;
	//メッシュで指定されたシェーダーの名前
	std::string mShaderName;
	//オブジェクト空間での境界球の半径を記録
	float mRadius;
	//表面の鏡面反射力
	float mSpecPower;
	// 法線マップを使用するかどうかのフラグ
	bool mUseNormalMap;

	AABB mBox;
};
