////////////////////////////////////////////////////////////////////////////
// 頂点レイアウト
// 
// PosNormTex = 8 * sizeof(float) = 32 bytes
// |   位置    |   法線    |Texture|
// | x | y | z | x | y | z | u | v | 
//
// posNormSkinTex
// PosNormSkinTex = (8 * sizeof(float)) + (8 * sizeof(char)) = 40 bytes
// |   位置    |   法線     |  Bones  | weight  |Texture|
// | x | y | z | x | y | z  | char[4] | char[4] | u | v |
// 
// PosNormTangentTex
// PosNormTangentTex = 11 * sizeof(float) = 44bytes
// |    位置   |   法線    |   uv   |   従法線  |
// | x | y | z | x | y | z | u | v  | x | y | z |
// 
// ※weightの確保はcharだが、精度が必要ないので8bit固定小数として使用する
////////////////////////////////////////////////////////////////////////////
#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const void* verts, unsigned int numVerts, Layout layout,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	,mNumIndices(numIndices)
{
	if (layout == PosNormTangentTex)
	{
		Vector3 destPos1, destPos2, destPos3;
		Vector2 destUV1, destUV2, destUV3;
		Vector3 destTangent;
		
		const float* vert = static_cast<const float*>(verts);

		for (int i = 0; i < *indices - 2; i++)
		{
			// 頂点配列から座標を取り出す
			GetPosVec(destPos1, vert, i);
			GetPosVec(destPos2, vert, i);
			GetPosVec(destPos3, vert, i);

			// 頂点配列からuv座標を取り出す
			GetUVVec(destUV1, vert, i);
			GetUVVec(destUV2, vert, i);
			GetUVVec(destUV3, vert, i);

			// タンジェントベクトルの計算
			CalcTangent(destTangent, destPos1, destPos2, destPos3, destUV1, destUV2, destUV3);
		}
	}

	// 頂点配列の作成
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	unsigned vertexSize = 8 * sizeof(float);
	
	// 頂点レイアウトがスケルタルモデルならボーンID、影響度分サイズを増やす
	if (layout == PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}
	// 頂点レイアウトが法線マップを使うモデルなら従法線分サイズを増やす
	if (layout == PosNormTangentTex)
	{
		vertexSize = 8 * sizeof(float) + 3 * sizeof(float);
	}

	// 頂点バッファの作成
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * vertexSize, verts, GL_STATIC_DRAW);

	// インデックスバッファの作成
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	
	// 頂点属性をGLに渡す
	if (layout == PosNormTex)
	{
		// float 3個分　→　位置 x,y,z　位置属性をセット
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// 次のfloat 3個分 → 法線 nx, ny, nz　法線属性をセット
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		// 次のfloat 2個分 u, v  テクスチャ座標属性をセット
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
	}
	else if (layout == PosNormSkinTex)
	{
		// float 3個分　→　位置 x,y,z　位置属性をセット
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// 次のfloat 3個分 → 法線 nx, ny, nz　法線属性をセット
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		//　影響受けるボーンインデックス番号  (int型をキープ)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
		// ボーンウェイト情報 (float に変換)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));
		// 次のfloat 2個分 u, v  テクスチャ座標属性をセット
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
	}
	else if (PosNormTangentTex)
	{
		// float 3個分　→　位置 x,y,z　位置属性をセット
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		// 次のfloat 3個分 → 法線 nx, ny, nz　法線属性をセット
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));

		// 次のfloat 2個分 u, v  テクスチャ座標属性をセット
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));

		// 次のfloat 3個分 → 従法線 nx, ny, nz　従法線属性をセット
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 8));
	}
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::CreateCubeMapVAO()
{
	float cubeMapVertices[] =
	{
		-1.0f, -1.0f,  1.0f, // 0         7-------6
		 1.0f, -1.0f,  1.0f, // 1        /|      /|
		 1.0f, -1.0f, -1.0f, // 2       4-------5 |
		-1.0f, -1.0f, -1.0f, // 3       | |     | |
		-1.0f,  1.0f,  1.0f, // 4       | 3 - - | 2
		 1.0f,  1.0f,  1.0f, // 5       |/      |/
		 1.0f,  1.0f, -1.0f, // 6       0-------1
		-1.0f,  1.0f, -1.0f  // 7
	};

	unsigned int cubeMapIndices[] =
	{
		// right
		1,2,6,
		6,5,1,
		// left
		0,4,7,
		7,3,0,
		// top
		4,5,6,
		6,7,4,
		// bottom
		0,3,2,
		2,1,0,
		//back
		0,1,5,
		5,4,0,
		// front
		3,7,6,
		6,2,3
	};

	unsigned int cubeMapVBO, cubeMapEBO;

	// 頂点配列の作成
	glGenVertexArrays(1, &mCubeMapVertexArray);
	glBindVertexArray(mCubeMapVertexArray);

	// 頂点バッファの作成
	glGenBuffers(1, &cubeMapVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeMapVertices), &cubeMapVertices, GL_STATIC_DRAW);

	// インデックスバッファの作成
	glGenBuffers(1, &cubeMapEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeMapEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeMapIndices), &cubeMapIndices, GL_STATIC_DRAW);

	// Attribute 0
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexArray::CalcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3,
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

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}

void VertexArray::SetActiveCubeMap()
{
	glBindVertexArray(mCubeMapVertexArray);
}

void VertexArray::SetTangent(float* destfv, int index, const Vector3& tangent)
{
	destfv[index * 11 + 8 + 0] = tangent.x;
	destfv[index * 11 + 8 + 1] = tangent.y;
	destfv[index * 11 + 8 + 2] = tangent.z;
}

void VertexArray::GetPosVec(Vector3& destPos, const float* fv, int index)
{
	destPos.x = fv[index * 11 + 0];
	destPos.y = fv[index * 11 + 1];
	destPos.z = fv[index * 11 + 2];
}

void VertexArray::GetUVVec(Vector2& destUV, const float* fv, int index)
{
	destUV.x = fv[index * 11 + 6 + 0];
	destUV.y = fv[index * 11 + 6 + 1];
}
