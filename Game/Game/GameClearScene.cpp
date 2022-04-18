#include "GameClearScene.h"
#include "BitmapText.h"
#include "Texture.h"
#include "Input.h"
#include "TitleScene.h"

GameClearScene::GameClearScene()
{
	printf("--------------GameClearScene---------------\n");

    //フォントの初期化
    mFont = new BitmapText;
    mFont->SetFontImage(16, 6, "Assets/Font/font.png");
    mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

    mGameClearTex = new Texture;
    mGameClearTex = RENDERER->GetTexture("Assets/Scene/GameClear.png");
}

GameClearScene::~GameClearScene()
{
}

SceneBase* GameClearScene::Update()
{
    if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
    {
        return new TitleScene;
    }
    return this;
}

void GameClearScene::Draw()
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
    RENDERER->DrawTexture(mGameClearTex, Vector2(960, 540), 1.0f, 1.0f);

    // 2D描画の終了処理
    RENDERER->SpriteDrawEnd();

    // 画面のフリップ
    GAMEINSTANCE.GetRenderer()->WindowFlip();
}
