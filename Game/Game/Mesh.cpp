#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL_log.h>
#include "Math.h"

namespace 
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
}

namespace
{
	/// <summary>
	/// 頂点配列に従法線をセット
	/// </summary>
	/// <param name="vertices">頂点配列</param>
	/// <param name="index">頂点配列のインデックス</param>
	/// <param name="tangent">セットする従法線</param>
	void SetTangent(std::vector<Vertex>& vertices, int index, const Vector3& tangent)
	{
		vertices[index * 11 + 8 + 0].f = tangent.x;
		vertices[index * 11 + 8 + 1].f = tangent.y;
		vertices[index * 11 + 8 + 2].f = tangent.z;
	}

	/// <summary>
	/// 頂点配列から頂点座標を取得
	/// </summary>
	/// <param name="destPos">頂点座標を格納する変数</param>
	/// <param name="vertices">頂点配列</param>
	/// <param name="index">頂点配列のインデックス</param>
	void GetPosVec(Vector3& destPos, const std::vector<Vertex>& vertices, int index)
	{
		destPos.x = vertices[index * 11 + 0].f;
		destPos.y = vertices[index * 11 + 1].f;
		destPos.z = vertices[index * 11 + 2].f;
	}

	/// <summary>
	/// 頂点配列からUV座標を取得
	/// </summary>
	/// <param name="destUV">UV座標を格納する変数</param>
	/// <param name="vertices">頂点配列</param>
	/// <param name="index">頂点配列のインデックス</param>
	void GetUVVec(Vector2& destUV, const std::vector<Vertex>& vertices, int index)
	{
		destUV.x = vertices[index * 11 + 6 + 0].f;
		destUV.y = vertices[index * 11 + 6 + 1].f;
	}
}

Mesh::Mesh()
	:mVertexArray(nullptr)
	,mRadius(0.0f)
	,mSpecPower(100.0f)
	,mUseNormalMap(false)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Load(const std::string& fileName, Renderer* renderer)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("ファイルが見つかりませんでした: Mesh %s", fileName.c_str());
		return false;
	}
	// JSONの解析を行う
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonstr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonstr);

	// JSONオブジェクトか？
	if (!doc.IsObject())
	{
		SDL_Log("Mesh &s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	//バージョンのチェック
	if (ver != 1)
	{
		SDL_Log("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	mShaderName = doc["shader"].GetString();

	// 頂点レイアウトとサイズをファイルからセット
	VertexArray::Layout layout = VertexArray::PosNormTex;
	size_t vertSize = 8;

	std::string vertexFormat = doc["vertexformat"].GetString();
	if (vertexFormat == "PosNormSkinTex")
	{
		layout = VertexArray::PosNormSkinTex;
		// This is the number of "Vertex" unions, which is 8 + 2 (for skinning)s　1個の頂点の集合の数　８　＋　２（スキニング分）
		// 3 (位置xyz) + 3(法線xyz) + 2(Boneと重み）＋　2(UV)  の計　10個分（40byte) 　
		vertSize = 10;
	}
	if (vertexFormat == "PosNormTexTangent")
	{
		layout = VertexArray::PosNormTexTangent;
		vertSize = 11;
	}

	// テクスチャのロード
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName.c_str());
		return false;
	}

	// スペキュラー強度
	mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

	// テクスチャ読み込み
	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		// このテクスチャは既に読みこんでるか？
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr)
		{
			// テクスチャ読み込みのトライ
			t = renderer->GetTexture(texName);
			if (t == nullptr)
			{
				// NULLのままであればデフォルトテクスチャをセット
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		mTextures.emplace_back(t);
	}

	//頂点読み込み
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh &s has no vertices", fileName.c_str());
		return false;
	}

	// 頂点データ
	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	mRadius = 0.0f;

	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// 現時点で、８つの要素(位置xyz 法線xyz テクスチャuvの要素）が入っている
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			printf("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		// 頂点位置から　バウンディングスフィアの半径を計算する（原点からの距離が最大のものを半径とする）
		Vector3 pos(static_cast<float>(vert[0].GetDouble()),
			        static_cast<float>(vert[1].GetDouble()),
			        static_cast<float>(vert[2].GetDouble()));
		mRadius = Math::Max(mRadius, pos.LengthSq());

		// バウンディングボックスも計算
		if (i == 0)
		{
			mBox.InitMinMax(pos);
		}
		mBox.UpdateMinMax(pos);

		// 頂点レイアウトが PosNormTexなら（ボーンデータ無し）
		if (layout == VertexArray::PosNormTex)
		{
			Vertex v;
			// Add the floats　float値を追加
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
		//頂点レイアウトが PosNormSkinTexなら (ボーンデータ有り)
		else if(layout == VertexArray::PosNormSkinTex)
		{
			Vertex v;
			// Add pos/normal　頂点と法線を追加　6個分
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// Add skin information　スキン情報追加（ボーン番号:unsigned charの1バイト分）
			for (rapidjson::SizeType j = 6; j < 14; j += 4)  //ループとしては2回転する　1回転目はボーン番号、2回転目はボーンウェイトをintとして取ってくる（使用時に浮動小数化するっぽい）
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// Add tex coords　テクスチャ座標
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
		// 頂点レイアウトがPosNormTangentTexなら
		else if (layout == VertexArray::PosNormTexTangent)
		{
			Vertex v;
			Vector3 destTangent;

			// 頂点と法線とUV座標を追加　8個分
			for (rapidjson::SizeType j = 0; j < 8; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// 空のdestTangentを追加(destTangentのサイズ分配列を確保) 3個分
			v.f = static_cast<float>(destTangent.x);
			vertices.emplace_back(v);
			v.f = static_cast<float>(destTangent.y);
			vertices.emplace_back(v);
			v.f = static_cast<float>(destTangent.z);
			vertices.emplace_back(v);
		}
	}


	// バウンディングスフィアの半径を計算
	mRadius = Math::Sqrt(mRadius);

	//頂点インデックスをロード
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	//頂点インデックスを引っ張ってくる
	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// 頂点レイアウトがPosNormTangentTexなら従法線を計算する
	if (layout == VertexArray::PosNormTexTangent)
	{
		Vector3 destPos1,destPos2,destPos3;
		Vector2 destUV1, destUV2, destUV3;
		Vector3 destTangent;
		for (int i = 0; i < vertices.size() / vertSize; i += 3)
		{
			// ３頂点の座標を取得
			GetPosVec(destPos1, vertices, i + 0);
			GetPosVec(destPos2, vertices, i + 1);
			GetPosVec(destPos3, vertices, i + 2);

			// 3頂点のUV座標を取得
			GetUVVec(destUV1, vertices, i + 0);
			GetUVVec(destUV2, vertices, i + 1);
			GetUVVec(destUV3, vertices, i + 2);

			// 従法線の計算
			CalcTangent(destTangent, destPos1, destPos2, destPos3, destUV1, destUV2, destUV3);

			// 従法線を頂点配列にセット
			SetTangent(vertices,i + 0,destTangent);
			SetTangent(vertices,i + 1,destTangent);
			SetTangent(vertices,i + 2,destTangent);
		}
	}

	// 頂点配列を作成する
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / static_cast<unsigned>(vertSize),
		                           layout,indices.data(), static_cast<unsigned>(indices.size()));

	return true;
}

void Mesh::Unload()
{
	delete mVertexArray;
	mVertexArray = nullptr;
}

void Mesh::CalcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, 
	                   const Vector2& uv1, const Vector2& uv2, const Vector2& uv3)
{
	Vector3 edge1, edge2;
	edge1 = pos2 - pos1;
	edge2 = pos3 - pos1;

	Vector2 deltaUV1 = uv2 - uv1;
	Vector2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	destTangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	destTangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	destTangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	destTangent.Normalize();
}

Texture* Mesh::GetTexture(size_t index)
{
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}