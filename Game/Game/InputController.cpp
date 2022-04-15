#include "InputController.h"
#include <limits.h>
#include <iostream>

//// XBox パッドのしきい値
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

InputController::InputController()
	: mpGameController(nullptr)
	, mWichController(-1)
{
	// ボタンと軸を0で初期化
	memset(mButtonStates, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
	memset(mButtonStatesPrev, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
	memset(mAxisValues, 0, sizeof(float) * SDL_CONTROLLER_AXIS_MAX);
}

// 初期化
bool InputController::Initialize(void)
{
	// パッドの設定ファイル gamecontrollerdb.txt の読み込みと問題が無いかのチェック
	int iNumOfControllers = SDL_GameControllerAddMappingsFromFile("Assets/PadSetting/gamecontrollerdb.txt");
	if (iNumOfControllers == -1)
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Error loading database [%s]", SDL_GetError());
		return false;
	}

	// コントローラ開く
	mpGameController = SDL_GameControllerOpen(0);
	if (!mpGameController)
	{
		return false;
	}

	if (SDL_IsGameController(0))
	{
		std::cout << SDL_GameControllerMapping(mpGameController) << std::endl;
	}

	// コントローライベントの無視（こちらからフレーム毎に状態を取得するため）
	SDL_GameControllerEventState(SDL_IGNORE);
	return true;
}

// コントローラ更新処理
void InputController::Update(void)
{
	// コントローラが無い場合は early exitする
	if (mpGameController == NULL)
	{
		return;
	}

	// 前のフレームのコントローラの状態をコピーする
	memcpy(&mButtonStatesPrev, &mButtonStates, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);

	// コントローラの状態を更新する
	SDL_GameControllerUpdate();

	// 現在のコントローラのボタン状態を保存
	for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; ++b)
	{
		mButtonStates[b] = SDL_GameControllerGetButton(mpGameController, (SDL_GameControllerButton)b);
	}

	// 現在のコントローラの軸情報を保存
	for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; ++a)
	{
		mAxisValues[a] = SDL_GameControllerGetAxis(mpGameController, (SDL_GameControllerAxis)a);
	}

	// L Stick入力をベクトル化
	const float maxInput = 32767.0f;
	mLAxis.x = (float)mAxisValues[SDL_CONTROLLER_AXIS_LEFTX];
	mLAxis.y = (float)mAxisValues[SDL_CONTROLLER_AXIS_LEFTY];
	mLAxis.x = (fabs(mLAxis.x) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0.0f : mLAxis.x / maxInput;
	mLAxis.y = (fabs(mLAxis.y) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0.0f : mLAxis.y / maxInput;

	// R Stick入力をベクトル化
	mRAxis.x = (float)mAxisValues[SDL_CONTROLLER_AXIS_RIGHTX];
	mRAxis.y = (float)mAxisValues[SDL_CONTROLLER_AXIS_RIGHTY];
	mRAxis.x = (fabs(mRAxis.x) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0.0f : mRAxis.x / maxInput;
	mRAxis.y = (fabs(mRAxis.y) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0.0f : mRAxis.y / maxInput;

	// ボタンを解釈する
	for (unsigned int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		Uint8 nowInput = mButtonStates[i];
		Uint8 prevInput = mButtonStatesPrev[i];

		// 現在押されていたら・・・
		if (nowInput)
		{
			//過去のキー状態によって押した瞬間か、押されっぱなし判断
			if (prevInput)
			{
				mButtonFinalStates[i] = KEY_STATE_PRESSED;
			}
			else
			{
				mButtonFinalStates[i] = KEY_STATE_PUSHDOWN;
			}
		}
		else
		{
			// 過去キーより、いま離したか、押されていないか
			if (prevInput)
			{
				mButtonFinalStates[i] = KEY_STATE_PULLUP;
			}
			else
			{
				mButtonFinalStates[i] = KEY_STATE_OFF;
			}
		}
	}

}

void InputController::Release(void)
{
	if (!mpGameController)
	{
		SDL_GameControllerClose(mpGameController);
	}
	mpGameController = nullptr;
}

void InputController::ReceiveEvent(const SDL_Event& oEvent)
{
	switch (oEvent.type)
	{
		// コントローラ追加イベント
	case SDL_CONTROLLERDEVICEADDED:
		// コントローラが取り付けられていなかった場合
		if (mpGameController != NULL)
		{
			// コントローラのオープン
			mWichController = oEvent.cdevice.which;
			mpGameController = SDL_GameControllerOpen(mWichController);
			// 前に取り付けられたコントローラの問題を回避するためメモリを0にセット。
			memset(mButtonStates, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
			memset(mButtonStatesPrev, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
			memset(mAxisValues, 0, sizeof(float) * SDL_CONTROLLER_AXIS_MAX);
		}
		break;

		// コントローラ取り外しイベント
	case SDL_CONTROLLERDEVICEREMOVED:

		// 現在のコントローラが取り外されかのチェック
		if (mWichController == oEvent.cdevice.which)
		{
			mWichController = -1;
			mpGameController = NULL;
		}
		break;
	}
}

bool InputController::IsControllerButtonTriggered(SDL_GameControllerButton iButton) const
{
	return (mButtonStates[iButton] == 1 && mButtonStatesPrev[iButton] == 0);
}

bool InputController::IsControllerButtonPressed(SDL_GameControllerButton iButton) const
{
	return (mButtonStates[iButton] == 1);
}

bool InputController::IsControllerButtonReleased(SDL_GameControllerButton iButton) const
{
	return (mButtonStates[iButton] == 0 && mButtonStatesPrev[iButton] == 1);
}

float InputController::GetAxisValue(SDL_GameControllerAxis iAxis) const
{
	return mAxisValues[iAxis];
}

KEY_STATE_ENUM InputController::GetInput(SDL_GameControllerButton key)
{
	return mButtonFinalStates[key];
}

