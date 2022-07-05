#include "GameScene.h"
#include "Game.h"
#include <iostream>
#include "Math.h"
#include "Renderer.h"
#include "DebugGrid.h"
#include "AudioManager.h"
#include "BitmapText.h"
#include "Input.h"
#include "PhysicsWorld.h"
#include "Texture.h"
#include "StaticBGActor.h"
#include "BGCollisionSetter.h"
#include "PlayerActor.h"
#include "EnemyActor.h"
#include "ThirdPersonCameraActor.h";
#include "Gun.h"
#include "SphereObj.h"


GameScene::GameScene()
	:mFont(nullptr)
	,mTex (nullptr)
	,mGrid(nullptr)
	,mHealthScaleX(0.3f)
	,mLightDistance(4000.0f)
{
	printf("-----------------GameScene-----------------\n");
	// フォント初期化
	mFont = new BitmapText;
	mFont->SetFontImage(16, 6, "Assets/Font/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	// 行列初期化
	Matrix4 proj;
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f), GAMEINSTANCE.GetRenderer()->GetScreenWidth(), GAMEINSTANCE.GetRenderer()->GetScreenHeight(), 1.0, 10000.0f);
	GAMEINSTANCE.GetRenderer()->SetProjMatrix(proj);

	//デバッググリッド
	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid(2000, 20, color);

	// ライティング設定
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.1f, 0.1f, 0.1f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, 0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);
	Vector3 lightDir = dir.mDirection;
	RENDERER->SetDepthSetting(Vector3(890, -130, 20), lightDir, Vector3::UnitZ, mLightDistance);

	mTex = new Texture;
	mTex = RENDERER->GetTexture("Assets/Player/Health.png");

	// プレイヤーの生成
	player = new PlayerActor(Vector3(890, -50, -55),                 // 座標
	                         1.0f,                                  // スケール
	                         "Assets/Player/SpecialForces.gpmesh",  // gpMeshのファイルパス
	                         "Assets/Player/SpecialForces.gpskel"); // gpSkelのファイルパス
	
	// 銃の生成
	Gun* gun = new Gun(Vector3(90, -40, 140),                         // オフセット位置
		               Vector3(-15, 130, 0),                          // オフセット角度(radian)
		               "Assets/Gun/SK_KA47.gpmesh",                   // gpMeshのファイルパス
		                player->GetSkeltalMeshComp(),                 // アタッチ先のSkeltalMeshCompクラスのポインタ
		               "LeftHandIndex4");                             // アタッチ先のボーン名

    // カメラの生成
	ThirdPersonCameraActor* camera = new ThirdPersonCameraActor(player);
	camera->SetCameraLength(800.0f);

	// バックグラウンドの生成
	new StaticBGActor(Vector3(890, -130, 20), "Assets/BackGround/testMap.gpmesh");
	
	new SphereObj(Vector3(890, -50, 20));

	// バックグラウンドの当たり判定の生成
	new BGCollisionSetter("Assets/BackGround/CollisionBox.json");

	
	// ゲームシステムに当たり判定リストを登録する
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::Enemy, Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround, Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround, Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::PlayerBullet, Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::EnemyBullet, Tag::Player);
}

GameScene::~GameScene()
{
	delete mGrid;
}

SceneBase* GameScene::Update()
{
	static float time = 0;
	time += 0.01f;

	// 当たり判定表示モードの切り替え
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();
	}

	DirectionalLight dirLight = RENDERER->GetDirectionalLight();
	Vector3 lightDir = dirLight.mDirection;
	Vector3 playerPos = player->GetPosition();
	RENDERER->SetDepthSetting(Vector3(890, -130, 20), lightDir, Vector3::UnitZ, mLightDistance);

	RENDERER->GetEffekseerManager()->Update();
	
	return this;
}

void GameScene::Draw()
{
	glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// ゲームシステム関連の描画（コンポーネント系のものはここですべて描画される)
	GAMEINSTANCE.GetRenderer()->Draw();

	// 2D描画の開始処理
	RENDERER->SpriteDrawBegin();

	// 体力ゲージ描画
	/*mHealthScaleX = mAltar->GetHealth() / 1000 * 3.5f;
	if (mHealthScaleX <= 0.001)
	{
		mHealthScaleX = 0;
	}
	RENDERER->DrawHelthGauge(mTex, Vector2(10, 50), mHealthScaleX, 0.03f, 1.0f);*/
	//RENDERER->DrawHelthGauge(mUI, Vector2(1500, 540),1.0f ,1.0f, 1.0f);

	// 2D描画の終了処理
	RENDERER->SpriteDrawEnd();

// エフェクト関連の処理 //
	// エフェクト描画の開始処理
	RENDERER->GetEffekseerRenderer()->BeginRendering();
	RENDERER->GetEffekseerManager()->Draw();
	// エフェクト描画の終了処理
	RENDERER->GetEffekseerRenderer()->EndRendering();

	// 画面フリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
