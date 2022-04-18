#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DepthMap
{
public:
	DepthMap();
	~DepthMap();
	void CreateDepthTexture();
private:
	unsigned int mDepthMapFBO;

	unsigned int mDepthMap;

	const unsigned int SHADOW_WIDTH  = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;
};