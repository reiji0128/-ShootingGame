#include "DebugGrid.h"
#include "Shader.h"

DebugGrid::DebugGrid(float size, int split, Vector3& color)
	: mIsReady(false)
	, mColor(color)
{
	int lineNum = split + 1;
	float halfSize = size * 0.5f;
	float splitLen = size / split;

	// 頂点個数　＝　分割時のライン本数　＊　２（縦と横）　* ２（1本につき2頂点）
	mNumVerts = lineNum * 2 * 2;

	//頂点バッファへ流し込むラインデータ作成
	mVertexData = new GLfloat[mNumVerts * 3];

	// 縦方向
	int i;
	for (i = 0; i < lineNum; i++)
	{
		mVertexData[(i * 6) + 0] = splitLen * i - halfSize;
		mVertexData[(i * 6) + 1] = -halfSize;;
		mVertexData[(i * 6) + 2] = 0.0f;
		mVertexData[(i * 6) + 3] = splitLen * i - halfSize;
		mVertexData[(i * 6) + 4] = +halfSize;
		mVertexData[(i * 6) + 5] = 0.0f;
	}
	int ofs = lineNum * 6;
	// 横方向
	for (i = 0; i < lineNum; i++)
	{
		mVertexData[ofs + (i * 6) + 0] = -halfSize;
		mVertexData[ofs + (i * 6) + 1] = splitLen * i - halfSize;
		mVertexData[ofs + (i * 6) + 2] = 0.0f;
		mVertexData[ofs + (i * 6) + 3] = +halfSize;
		mVertexData[ofs + (i * 6) + 4] = splitLen * i - halfSize;
		mVertexData[ofs + (i * 6) + 5] = 0.0f;
	}
	if (Init())
	{
		mIsReady = true;
	}
}

DebugGrid::~DebugGrid()
{
	delete[] mVertexData;
}

void DebugGrid::Draw()
{
	glBindVertexArray(mVertexArray);

	mShader->SetActive();
	Matrix4 mWorld, mView, mProj;
	mView = GAMEINSTANCE.GetRenderer()->GetViewMatrix();
	mProj = GAMEINSTANCE.GetRenderer()->GetProjectionMatrix();

	mShader->SetMatrixUniform("uViewProj", mView * mProj);
	mShader->SetVectorUniform("uColor", mColor);

	glDrawArrays(GL_LINES, 0, mNumVerts);
}

bool DebugGrid::Init()
{
	// 頂点配列の作成
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// 頂点バッファの作成
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mNumVerts * sizeof(GLfloat) * 3, mVertexData, GL_STATIC_DRAW);
	GLErrorHandle("DebugGrid::Init()");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);

	mShader = new Shader;
	if (!mShader->Load("Shaders/Line.vert", "Shaders/Line.frag"))
	{
		return false;
	}
	GLErrorHandle("DebugGrid::Init() ShaderLoad");

	return true;
}
