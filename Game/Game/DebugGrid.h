#pragma once

#include "Game.h"
#include "Renderer.h"

class DebugGrid
{
public:
	DebugGrid(float size, int split, Vector3& color);
	~DebugGrid();
	void Draw();

private:
	bool Init();

	bool mIsReady;
	unsigned int  mNumVerts;
	unsigned int  mVertexBuffer;
	unsigned int  mVertexArray;

	GLfloat* mVertexData;

	GLuint        mVertShader;
	GLuint        mFragShader;
	GLuint        mShaderProgram;
	Vector3       mColor;

	class Shader* mShader;
};