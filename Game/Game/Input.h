#pragma once
#include "Game.h"
#include "InputController.h"

class InputController;


// キーコンフィグ
typedef enum KEY_CONFIG
{
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	KEY_L,
	KEY_R,
	KEY_START,
	KEY_SELECT,
	KEY_W,
	KEY_S,
	KEY_D,
	KEY_A,
	KEY_SPACE,

	KEY_TYPE_ENUM_MAX,    // キー種類の総数
}KEY_CONFIG;

// キーの状態
typedef enum KEY_STATE_ENUM
{
	KEY_STATE_OFF = 0,    // 押されていない
	KEY_STATE_PUSHDOWN,   // 押下
	KEY_STATE_PRESSED,    // 押され続けている
	KEY_STATE_PULLUP      // 押離し
}KEY_STATE_ENUM;

class Input
{
private:

	/// <summary>
	/// シングルトン
	/// </summary>
	Input();

public:

	/// <summary>
	/// InputInstanceクラスのインスタンスを返す
	/// </summary>
	/// <returns>InputInstanceクラスのインスタンス</returns>
	static Input& InputInstance()
	{
		static Input InputInstance;
		return InputInstance;
	}

	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Input() {};

	/// <summary>
	/// 入力状態の更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// あるキーが押され続けられている状態か？
	/// </summary>
	/// <param name="SDL_SCANCODE_key">キーの番号</param>
	/// <returns></returns>
	bool IsKeyPressed(Uint8 SDL_SCANCODE_key);
	bool IsKeyPullup(Uint8 SDL_SCANCODE_key);      // あるキーが押し上げられたか？
	bool IsKeyPushdown(Uint8 SDL_SCANCODE_key);    // あるキーは押され続けられているか？
	bool IsKeyOff(Uint8 SDL_SCANCODE_key);         // あるキーは押されていないか？
	bool IsKeyPressed(KEY_CONFIG key);     // あるキーが押され続けられている状態か？
	bool IsKeyPullup(KEY_CONFIG key);      // あるキーが押し上げられたか？
	bool IsKeyPushdown(KEY_CONFIG key);    // あるキーは押され続けられているか？
	bool IsKeyOff(KEY_CONFIG key);         // あるキーは押されていないか？

	char GetInput(Uint8 SDL_SCANCODE_key);         // あるキーの入力状態の取得
	KEY_STATE_ENUM GetInput(KEY_CONFIG key);// キーコンフィグされたキーの状態を取得する

	InputController* GetControllerInstance();      // コントローラインスタンス
	bool ControllerInit();

	/// <summary>
	/// Lスティックの移動量の取得
	/// </summary>
	/// <returns>Lスティックの移動量</returns>
	Vector2& GetLStick() { return mLStick; }

	/// <summary>
	/// Rスティックの移動量の取得
	/// </summary>
	/// <returns>Rスティックの移動量</returns>
	Vector2& GetRStick() { return mRStick; }

	/// <summary>
	/// Lスティックが動かされたか
	/// </summary>
	/// <returns></returns>
	bool IsLStickMove() { return mIsLStickMove; }

	/// <summary>
	/// Rスティックが動かされたか
	/// </summary>
	/// <returns></returns>
	bool IsRStickMove() { return mIsRStickMove; }

private:
	KEY_STATE_ENUM mKey[256];                      // 全キー状態取得用
	char mKeyState[2][256];                        // キー状態 [現在/過去フレーム][全キーコード]

	SDL_GameControllerButton mPadConfig[KEY_TYPE_ENUM_MAX];  // パッドコンフィグ
	KEY_STATE_ENUM mKeyConfigedKeyState[KEY_TYPE_ENUM_MAX];  // キーコンフィグ後のキー
	SDL_Scancode   mKeyConfig[KEY_TYPE_ENUM_MAX];            // キーコンフィグ

	int  mNowKey, mPrevKey;                        // 現フレーム、過去フレームのインデックス
	InputController* mController;                 // コントローラクラス

	// アナログコントローラーのLスティック
	Vector2 mLStick;

	// アナログコントローラーのRスティック
	Vector2 mRStick;

	bool mIsLStickMove;

	bool mIsRStickMove;
};

#define INPUT_INSTANCE Input::InputInstance()
