#include "TitleScene.h"
#include "Math.h"
#include "GameScene.h"
#include "DebugGrid.h"
#include "AudioManager.h"
#include "BitmapText.h"
#include "Input.h"
#include "Texture.h"


/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene()
{
    printf("-----------------TitleScene-----------------\n");
    //フォントの初期化
    mFont = new BitmapText;
    mFont->SetFontImage(16, 6, "Assets/Font/font.png");
    mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

    mTitle = new Texture;
    mTitle = RENDERER->GetTexture("Assets/Scene/Title.png");
}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene()
{
}

/// <summary>
/// シーンの更新処理
/// </summary>
/// <returns></returns>
SceneBase* TitleScene::Update()
{
    if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
    {
        return new GameScene;
    }
    return this;
}

/// <summary>
/// シーンの描画処理
/// </summary>
void TitleScene::Draw()
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
    mFont->TextDraw(500, 400, "Title Scene.");
    mFont->TextDraw(500, 500, "START ENTER");
    RENDERER->DrawTexture(mTitle, Vector2(960, 540), 1.0f, 1.0f);

    // 2D描画の終了処理
    RENDERER->SpriteDrawEnd();

    // 画面のフリップ
    GAMEINSTANCE.GetRenderer()->WindowFlip();
}
