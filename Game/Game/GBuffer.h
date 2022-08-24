#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Math.h"
#include "Shader.h"

class Shader;

class GBuffer
{
public:
	GBuffer();

	~GBuffer();
	
	void GBufferRenderingBegin();

	void GBufferRenderingEnd();

	void CreateGbuffer();

	void InputGBufferToShader(Shader* shader);

	unsigned int GetGBufferID() { return mGbuffer; }



private:
	int mScreenWidth;

	int mScreenHeight;

	unsigned int mGbuffer;

	unsigned int mPosition, mNormal, mAlbedoSpec, mRenderBufferObject;
};