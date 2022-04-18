#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

class GameOverScene : public SceneBase
{
public:
	GameOverScene();
	~GameOverScene();
	SceneBase* Update();
	void Draw();

private:
	class Texture* mGameOverTex;
};