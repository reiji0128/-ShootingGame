#include "GameOverScene.h"
#include "Texture.h"
#include "Input.h"
#include "TitleScene.h"

GameOverScene::GameOverScene()
{
	printf("---------------GameOverScene---------------\n");
	mGameOverTex = new Texture;
	mGameOverTex = RENDERER->GetTexture("Assets/Scene/GameOver.png");
}

GameOverScene::~GameOverScene()
{
}

SceneBase* GameOverScene::Update()
{
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		return new TitleScene;
	}
	return this;
}

void GameOverScene::Draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // 画面のクリア
    GAMEINSTANCE.GetRenderer()->WindowClear();

    // ゲームシステム関連の描画(コンポーネント系のものはここですべて描画される
    GAMEINSTANCE.GetRenderer()->Draw();

    // 2D関連の描画
    // 2D描画の準備処理
    RENDERER->SpriteDrawBegin();

    // 2DSpriteやフォントをここに書く
    RENDERER->DrawTexture(mGameOverTex, Vector2(960, 540), 1.0f, 1.0f);

    // 2D描画の終了処理
    RENDERER->SpriteDrawEnd();

    // 画面のフリップ
    GAMEINSTANCE.GetRenderer()->WindowFlip();
}
