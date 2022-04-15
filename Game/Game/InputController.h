#pragma once
#include "Game.h"
#include "Input.h"

class Input;

class InputController
{
public:

	InputController();

	bool Initialize(void); // 初期化
	void Update(void);     // 更新
	void Release(void);    // 解放処理

	//コントローラ接続イベント＆解除イベント
	void ReceiveEvent(const SDL_Event& oEvent);

	/// ボタンの状態取得関数
	bool IsControllerButtonTriggered(SDL_GameControllerButton iButton) const;  // 今押された？
	bool IsControllerButtonPressed(SDL_GameControllerButton iButton) const;  // 押されている？
	bool IsControllerButtonReleased(SDL_GameControllerButton iButton) const; // 離された？
	bool IsControllerAvailable() { return mpGameController != nullptr; }

	/// 現在の軸の値を取得
	float GetAxisValue(SDL_GameControllerAxis iAxis) const;
	Vector2& GetLAxisVec() { return mLAxis; }
	Vector2& GetRAxisVec() { return mRAxis; }

	enum KEY_STATE_ENUM GetInput(SDL_GameControllerButton key);

private:
	// コントローラ情報
	SDL_GameController* mpGameController;
	int                 mWichController;

	// コントローラの状態情報
	Uint8 mButtonStates[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mButtonStatesPrev[SDL_CONTROLLER_BUTTON_MAX];
	KEY_STATE_ENUM mButtonFinalStates[SDL_CONTROLLER_BUTTON_MAX]; // ボタンの最終状態

	float mAxisValues[SDL_CONTROLLER_AXIS_MAX];

	Vector2 mLAxis;
	Vector2 mRAxis;
};
