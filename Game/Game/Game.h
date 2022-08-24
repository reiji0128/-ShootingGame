#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_types.h>
#include "SceneBase.h"
#include "math.h"
#include "Tag.h"
#include "CameraActor.h"
class Game
{
private:
	/// <summary>
	/// シングルトン
	/// </summary>
	Game();

	/// <summary>
	/// 入力処理
	/// </summary>
	void Input();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <returns></returns>
	int  Update();

	/// <summary>
	/// アクターの更新処理
	/// </summary>
	void ActorUpdate();

	/// <summary>
	/// アクターリスト表示（デバッグ用）
	/// </summary>
	void ShowActor();


public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Game();


	static Game& GetInstance()                     // インスタンス
	{
		static Game instance;
		return instance;
	}

	/// <summary>
	/// ゲーム実行処理
	/// </summary>
	void Run();

	/// <summary>
	/// ゲーム終了処理
	/// </summary>
	void Shutdown();

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
	bool Initialize(int screenWidth, int screenHeight, bool fullScreen = false);

// セッター //
	/// <summary>
	/// 開始シーンのセット
	/// </summary>
	/// <param name="pScene">SceneBaseのポインタ</param>
	void                             setFirstScene(SceneBase* pScene) { mNowScene = pScene; }
	
	void SetCameraActor(CameraActor* camera);

	void SetInActiveCameraActor(CameraActor* camera);

	/// <summary>
	/// ゲームをシャットダウン
	/// </summary>
	void                             setShutDown() { mIsRunning = false; }

// ゲッター //
	/// <summary>
	/// Rendererポインタの取得
	/// </summary>
	/// <returns>Rendererのポインタ</returns>
	class Renderer* GetRenderer() { return mRenderer; }

	
	/// <summary>
	/// 2DのSDLレンダラーの取得
	/// </summary>
	SDL_Renderer* GetSDLRenderer();

	/// <summary>
	/// デルタタイムの取得
	/// </summary>
	/// <returns>デルタタイム</returns>
	const float                      GetDeltaTime() { return mDeltaTime; }
	
	/// <summary>
	/// PhysicsWorldポインタを取得
	/// </summary>
	/// <returns>PhysicsWorldのポインタ</returns> 
	class PhysicsWorld* const        GetPhysics() { return mPhysicsWorld; }
	
	/// <summary>
	/// AudioManagerポインタを取得
	/// </summary>
	/// <returns>AudioManagerポインタ</returns> 
	class AudioManager* const        GetAudio() { return mAudio; }

	/// <summary>
	/// タグからアクターを取得
	/// </summary>
	/// <param name="type">取得するアクターのタグ</param>
	/// <returns>アクターのポインタ</returns>
	std::vector<class Actor*>const& GetActors(Tag type);

	/// <summary>
	/// プレイヤーアクターのポインタの取得
	/// </summary>
	/// <returns>プレイヤーアクターのポインタ</returns>
	class Actor* GetPlayerActor();

	/// <summary>
	/// タグの中にいる最初のアクターを返す
	/// </summary>
	/// <returns>最初のアクターのポインタ</returns>
	class Actor* GetFirstActor(Tag type);

	const Vector3& GetViewTarget();
	const Vector3& GetViewPos();

	/// <summary>
	/// アクターの追加
	/// </summary>
	/// <param name="actor">アクターのポインタ</param>
	void                             AddActor(class Actor* actor);

	/// <summary>
	/// アクターの削除
	/// </summary>
	/// <param name="actor">アクターのポインタ</param>
	void                             RemoveActor(class Actor* actor);

	/// <summary>
	/// すべてのアクターの削除
	/// </summary>
	void                             RemoveAllActor();


	/// <summary>
	/// アクタータイプのアクターは存在しているか？
	/// </summary>
	/// <param name="type">確認するアクターのタグ</param>
	/// <returns>アクタータイプのアクターの数</returns>
	bool       IsExistActorType(Tag type);

	/// <summary>
	/// アクターIDからアクターへのポインタを検索する
	/// </summary>
	/// <param name="searchActorID">検索するID</param>
	/// <returns>
	/// アクターへのポインタ
	/// </returns>
	class Actor* FindActorFromID(int searchActorID);


private:
	// Rendererのポインタ
	class Renderer* mRenderer;

	// CameraActorのポインタ
	class CameraActor* mActiveCamera;

	// SceneBaseのポインタ
	class SceneBase* mNowScene;

	// PhysicsWorldのポインタ(あたり判定システム)
	class PhysicsWorld* mPhysicsWorld;

	// AudioManagerのポインタ(サウンド関連)
	class AudioManager* mAudio;

	// ゲームループ回すか？
	bool  mIsRunning;

	// ポーズモード
	bool mIsPauseMode;

	// 1フレームの経過時間（秒単位）
	float    mDeltaTime;

	// 経過時間（ミリ秒単位）
	Uint32   mTicksCount;

	// 射影行列
	Matrix4  mViewMatrix;

	// アクター追加準備用配列
	std::vector<class Actor*> mPendingActors;

	// アクター配列
	std::unordered_map<Tag, std::vector<class Actor*>> mActors;
};

//Gameクラスのインスタンス
#define GAMEINSTANCE Game::GetInstance()

//Rendererクラスのポインタを取得
#define RENDERER     Game::GetInstance().GetRenderer()

//AudioManagerクラスのポインタを取得
#define AUDIO        Game::GetInstance().GetAudio()

