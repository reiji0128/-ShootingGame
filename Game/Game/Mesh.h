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

	AABB mBox;
};
