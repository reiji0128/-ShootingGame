#pragma once
#include "Component.h"
#include <SDL.h>

class SpriteComponent :public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }
private:
	class Texture* mTexture;
	int mDrawOrder;
	int mTexHeight;
	int mTexWidth;
};