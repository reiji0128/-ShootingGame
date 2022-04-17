#include "game.h"
#include "Input.h"
#include "renderer.h"
#include "SceneBase.h"
#include "Actor.h"
#include "Animation.h"
#include "Skeleton.h"
#include "PhysicsWorld.h"
#include "AudioManager.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <typeinfo>
#include <iostream>

/// <summary>
/// コンストラクタ
/// </summary>
Game::Game()
	: mRenderer(nullptr)
	, mNowScene(nullptr)
	, mAudio(nullptr)
	, mIsRunning(true)
	, mPhysicsWorld(nullptr)
	, mDeltaTime(0.0f)
	, mTicksCount(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Game::~Game()
{
}

/// <summary>
/// 入力処理
/// </summary>
void Game::Input()
{
	// 終了イベントのキャッチ
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			// パッドイベント取得
			INPUT_INSTANCE.GetControllerInstance()->ReceiveEvent(event);
		}
	}

	// キーボード入力更新
	INPUT_INSTANCE.Update();

	// ESCが押されたら終了
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_ESCAPE))
	{
		mIsRunning = false;
	}

	// アクターデバッグ
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F1))
	{
		ShowActor();
	}

	// レンダリングリソース表示
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F2))
	{
		GetRenderer()->ShowResource();
	}

	// ポーズモード移行／解除
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_BACKSPACE))
	{
		//ポーズモード切替
		mIsPauseMode = !mIsPauseMode;
		Actor::State changeState;
		if (mIsPauseMode)
		{
			changeState = Actor::EPaused; //ポーズ
		}
		else
		{
			changeState = Actor::EActive; //アクティブ
		}

		//全てのステートを変更する
		for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
		{
			for (auto itr : mActors[tag])
			{
				if (itr->GetState() != Actor::EDead)
				{
					itr->SetState(changeState);
				}
			}
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Game::Draw()
{
	if (!mNowScene)
	{
		return;
	}
	mNowScene->Draw();
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns></returns>
int Game::Update()
{
	// 16ミリ秒(= 60フレーム/秒)になるように、前のフレームからの16ミリ秒以上経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// 前のフレームから現在時刻までの経過時間算出(秒単位）
	mDeltaTime = (SDL_GetTicks() - mTicksCount + 16) / 1000.0f;

	//このフレームの開始時間保存（次のフレームまでの経過時間をはかるため）
	mTicksCount = SDL_GetTicks();

	// フレーム時間があまりにも経過している場合は0.05 → 20fps固定
	if (mDeltaTime > 0.05f)
	{
		mDeltaTime = 0.05f;
	}

	if (!mNowScene)
	{
		return 0;
	}
	SceneBase* tmpScene;

	// アクターの処理
	ActorUpdate();

	//あたり判定
	mPhysicsWorld->Collision();

	//シーンを実行
	tmpScene = mNowScene->Update();

	if (tmpScene != mNowScene)
	{
		delete mNowScene;

		//新規シーンを今のシーンに差し替え
		mNowScene = tmpScene;

		return 1;
	}
	return 0;
}

/// <summary>
/// アクターの更新処理
/// </summary>
void Game::ActorUpdate()
{
	//全てのアクターの更新
	for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		for (auto actor : mActors[tag])
		{
			actor->Update(mDeltaTime);
		}
	}

	//ペンディング中のアクターをアクティブアクターに移動
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		Tag t = pending->GetTag();

		mActors[t].emplace_back(pending);
	}
	mPendingActors.clear();

	// 全ての死んでいるアクターを一時保管
	std::vector<Actor*> deadActors;
	for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		for (auto actor : mActors[tag])
		{
			if (actor->GetState() == Actor::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}
	}

	// 死んでいるアクターをdelete(mActorからも消去)
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();
}

/// <summary>
/// アクターリスト表示（デバッグ用）
/// </summary>
void Game::ShowActor()
{
	size_t actorCount = 0;
	for (Tag tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		actorCount += mActors[tag].size();
	}
	printf("\n\n<--------------ActorList----------------->\n");
	printf("---------> Active Actor ( %zd ) <-----------\n", actorCount);

	for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		for (auto i : mActors[tag])
		{
			printf("mem [%p] : id: %d ", i, i->GetID());
			std::cout << typeid(*i).name() << "\n";
		}
	}

	printf("---------> Pending Actor ( %zd ) <-----------\n", mPendingActors.size());
	for (auto i : mPendingActors)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}

}

/// <summary>
/// ゲーム実行処理
/// </summary>
void Game::Run()
{
	// レンダラーが初期化されていなかったら初期化
	if (!mRenderer)
	{
		Initialize(1920, 1080,true);
	}
	// 開始シーンが定義されていなかったら終了
	if (!mNowScene)
	{
		printf("開始シーンが設定されていません");
		return;
	}

	// ゲームループ
	while (mIsRunning)
	{
		Input();
		if (Update())
		{
			continue; // シーン切り替え時のみ描画処理をさせないため
		}
		Draw();
	}
}

/// <summary>
/// ゲーム終了処理
/// </summary>
void Game::Shutdown()
{
	RemoveAllActor();

	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
}

/// <summary>
/// 画面初期化設定
/// </summary>
/// <param name="screenWidth">スクリーンの幅</param>
/// <param name="screenHeight">スクリーンの高さ</param>
/// <param name="fullScreen">フルスクリーンにするか</param>
/// <returns>
/// true  : 初期化成功
/// false : 初期化失敗
/// </returns>
bool Game::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("SDL初期化失敗: %s\n", SDL_GetError());
		return false;
	}

	// レンダラーの作成
	mRenderer = new Renderer;
	if (!mRenderer->Initialize(screenWidth, screenHeight, fullScreen))
	{
		printf("Rendererの初期化に失敗");
		delete mRenderer;
		return false;
	}

	// サウンドの初期化
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		delete mRenderer;
		return false;
	}
	printf("SDLMixer初期化完了\n");
	// SDLMixer API初期化　44100:音源の周波数 2:ステレオ 4096:内部バッファサイズ
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		delete mRenderer;
		Mix_CloseAudio();
		Mix_Quit();
		return false;
	}
	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}
	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI初期化完了\n");

	// AudioSystem作成
	mAudio = new AudioManager;

	// あたり判定システム作成
	mPhysicsWorld = new PhysicsWorld;

	// コントローラ初期化
	INPUT_INSTANCE.ControllerInit();

	mTicksCount = SDL_GetTicks();
	return true;
}

void Game::SetCameraActor(CameraActor* camera)
{
	printf("SetCamera [%p]\n", camera);
	mActiveCamera = camera;
}

void Game::SetInActiveCameraActor(CameraActor* camera)
{
	if (camera == mActiveCamera)
	{
		printf("Camera is inActive. change to default view.\n");
		mActiveCamera = nullptr;
	}

	mViewMatrix = Matrix4::CreateLookAt(Vector3(0, 0, 100),
			                            Vector3(0, 0, 0),
			                            Vector3(0, 0, 1));
}

	
/// <summary>
/// 2DのSDLレンダラーの取得
/// </summary>
/// <returns>Rendererのポインタ</returns>
SDL_Renderer* Game::GetSDLRenderer()
{
	return mRenderer->GetSDLRenderer();
}

/// <summary>
/// アクターの追加
/// </summary>
/// <param name="actor">アクターのポインタ</param>
void Game::AddActor(Actor* actor)
{
	// いったんペンディングアクターに追加
	mPendingActors.emplace_back(actor);
}

/// <summary>
/// アクターの削除
/// </summary>
/// <param name="actor">アクターのポインタ</param>
void Game::RemoveActor(Actor* actor)
{
	//ペンディングアクター内にいる？
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		//ペンディングアクターの最後尾にデータを移動して、データを消す
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
		return;
	}

	// アクティブアクター内にいる？
	auto tag = actor->GetTag();
	iter = std::find(mActors[tag].begin(), mActors[tag].end(), actor);
	if (iter != mActors[tag].end())
	{
		//アクティブアクターの最後尾にデータを移動して、データ消す
		std::iter_swap(iter, mActors[tag].end() - 1);
		mActors[tag].pop_back();
	}
}

/// <summary>
/// すべてのアクターの削除
/// </summary>
void Game::RemoveAllActor()
{
	// アクターの削除　（アクターを通じてコンポーネントも削除される）
	for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		while (!mActors[tag].empty())
		{
			delete mActors[tag].back();
		}
	}

	while (!mPendingActors.empty())
	{
		delete mPendingActors.back();
	}

}

/// <summary>
/// タグからアクターを取得
/// </summary>
/// <param name="type">取得するアクターのタグ</param>
/// <returns>アクターのポインタ</returns>
std::vector<class Actor*>const& Game::GetActors(Tag type)
{
	return mActors[type];
}

/// <summary>
/// プレイヤーアクターのポインタの取得
/// </summary>
/// <returns>プレイヤーアクターのポインタ</returns>
Actor* Game::GetPlayerActor()
{
	return GetFirstActor(Tag::Player);
}

/// <summary>
/// タグの中にいる最初のアクターを返す
/// </summary>
/// <returns>最初のアクターのポインタ</returns>
Actor* Game::GetFirstActor(Tag type)
{
	// アクティブリストをチェック
	if (mActors[type].size() != 0)
	{
		return mActors[type][0];
	}

	// ペンディングにいないかを検索
	for (auto iter = mPendingActors.begin(); iter != mPendingActors.end(); ++iter)
	{
		if ((*iter)->GetTag() == type)
		{
			return *iter;
		}
	}

	return nullptr;
}

const Vector3& Game::GetViewTarget()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
	}
	return mActiveCamera->GetViewTarget();
}

const Vector3& Game::GetViewPos()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
	}
	return mActiveCamera->GetViewPos();
}

/// <summary>
/// アクタータイプのアクターは存在しているか？
/// </summary>
/// <param name="type">確認するアクターのタグ</param>
/// <returns>アクタータイプのアクターの数</returns>
bool Game::IsExistActorType(Tag type)
{
	return mActors[type].size() != 0;
}

/// <summary>
/// アクターIDからアクターへのポインタを検索する
/// </summary>
/// <param name="searchActorID">検索するID</param>
/// <returns>
/// アクターへのポインタ
/// </returns>
Actor* Game::FindActorFromID(int searchActorID)
{
	// アクティブリスト内から検索
	for (Tag t = Tag::Begin; t != Tag::End; ++t)
	{
		for (auto item : mActors[t])
		{
			if (item->GetID() == searchActorID)
			{
				return item;
			}
		}
	}
	// ペンディングにいないかを検索
	for (auto iter = mPendingActors.begin(); iter != mPendingActors.end(); ++iter)
	{
		if ((*iter)->GetID() == searchActorID)
		{
			return *iter;
		}
	}
	return nullptr;
}