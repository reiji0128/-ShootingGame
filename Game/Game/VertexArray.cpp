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

VertexArray::VertexArray()
{
}

VertexArray::VertexArray(const void* verts, unsigned int numVerts, Layout layout,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	,mNumIndices(numIndices)
{
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
	if (layout == PosNormTexTangent)
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
	else if (PosNormTexTangent)
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
		glEnableVertexAttribArray(3);
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
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// 頂点バッファの作成
	glGenBuffers(1, &cubeMapVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeMapVertices), &cubeMapVertices, GL_STATIC_DRAW);

	// インデックスバッファの作成
	glGenBuffers(1, &cubeMapEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeMapEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeMapIndices), &cubeMapIndices, GL_STATIC_DRAW);

	// Attribute 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}