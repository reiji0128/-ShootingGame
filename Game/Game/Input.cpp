#include "Input.h"
#include "InputController.h"

const float stickTrigThreshold = 0.01f;

Input::Input()
	: mNowKey(0)
	, mPrevKey(1)
	, mController(nullptr)
{
	// キーボード初期化
	mNowKey = 0;
	mPrevKey = 1;

	// キーボード状態の初期化
	for (int i = 0; i < 256; i++)
	{
		mKey[i] = KEY_STATE_OFF;
		mKeyState[mNowKey][i] = KEY_STATE_OFF;
		mKeyState[mPrevKey][i] = KEY_STATE_OFF;
	}

	// キーコンフィグ（本当は外部から読み込みたいところ）
	mKeyConfig[BUTTON_A] = SDL_SCANCODE_SPACE;
	mKeyConfig[BUTTON_B] = SDL_SCANCODE_F;
	mKeyConfig[BUTTON_X] = SDL_SCANCODE_C;
	mKeyConfig[BUTTON_Y] = SDL_SCANCODE_V;
	mKeyConfig[KEY_L] = SDL_SCANCODE_Q;
	mKeyConfig[KEY_R] = SDL_SCANCODE_E;
	mKeyConfig[KEY_UP] = SDL_SCANCODE_UP;
	mKeyConfig[KEY_DOWN] = SDL_SCANCODE_DOWN;
	mKeyConfig[KEY_LEFT] = SDL_SCANCODE_LEFT;
	mKeyConfig[KEY_RIGHT] = SDL_SCANCODE_RIGHT;

	mKeyConfig[KEY_W] = SDL_SCANCODE_W;
	mKeyConfig[KEY_S] = SDL_SCANCODE_S;
	mKeyConfig[KEY_A] = SDL_SCANCODE_A;
	mKeyConfig[KEY_D] = SDL_SCANCODE_D;
	mKeyConfig[KEY_SPACE] = SDL_SCANCODE_SPACE;

	mKeyConfig[KEY_START] = SDL_SCANCODE_RETURN;
	mKeyConfig[KEY_SELECT] = SDL_SCANCODE_R;

	mPadConfig[BUTTON_A] = SDL_CONTROLLER_BUTTON_A;
	mPadConfig[BUTTON_B] = SDL_CONTROLLER_BUTTON_B;
	mPadConfig[BUTTON_X] = SDL_CONTROLLER_BUTTON_X;
	mPadConfig[BUTTON_Y] = SDL_CONTROLLER_BUTTON_Y;
	mPadConfig[KEY_L] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	mPadConfig[KEY_R] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	mPadConfig[KEY_UP] = SDL_CONTROLLER_BUTTON_DPAD_UP;
	mPadConfig[KEY_DOWN] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	mPadConfig[KEY_LEFT] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	mPadConfig[KEY_RIGHT] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	mPadConfig[KEY_START] = SDL_CONTROLLER_BUTTON_START;
}

void Input::Update()
{
	// 現在のキーと1つ前のキーの状態を入れ替える  0を１に、1を0に
	mNowKey ^= 1;
	mPrevKey = mNowKey ^ 1;

	//キー状態取得
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	memcpy(mKeyState[mNowKey], keys, sizeof(char) * 256);

	for (int i = 0; i < 256; i++)
	{
		char nowInput = mKeyState[mNowKey][i];
		char prevInput = mKeyState[mPrevKey][i];

		if (nowInput) //現在押されている
		{
			//前のフレームも押されていた？
			if (prevInput)
			{
				mKey[i] = KEY_STATE_PRESSED; // 押されっぱなし
			}
			else {
				mKey[i] = KEY_STATE_PUSHDOWN;// 今押された
			}

		}
		else // 現在キーは押されていない
		{
			// 前のフレームで押されていた？
			if (prevInput)
			{
				mKey[i] = KEY_STATE_PULLUP;
			}
			else {
				mKey[i] = KEY_STATE_OFF;
			}
		}
	}
	// コントローラアップデート
	mController->Update();
	mLStick = mController->GetLAxisVec();
	mRStick = mController->GetRAxisVec();

	// コントローラのスティックがたおされているか
	float stickMoveLen2;
	stickMoveLen2 = Vector2::Dot(mLStick, mLStick);
	mIsLStickMove = stickTrigThreshold < stickMoveLen2;

	stickMoveLen2 = Vector2::Dot(mRStick, mRStick);
	mIsRStickMove = stickTrigThreshold < stickMoveLen2;


	// キーコンフィグ状態
	for (unsigned int i = 0; i < KEY_TYPE_ENUM_MAX; i++)
	{
		mKeyConfigedKeyState[i] = mKey[mKeyConfig[i]];
	}

	// パッドの状態を得る。キーボードが先に入力されていたら無視
	for (unsigned int i = 0; i < KEY_TYPE_ENUM_MAX; i++)
	{
		if (mKeyConfigedKeyState[i] == KEY_STATE_OFF)
		{
			mKeyConfigedKeyState[i] = mController->GetInput(mPadConfig[i]);
		}
	}
}

char Input::GetInput(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key];
}

KEY_STATE_ENUM Input::GetInput(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key];
}

bool Input::IsKeyPressed(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key] == KEY_STATE_PRESSED;
}

bool Input::IsKeyPullup(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key] == KEY_STATE_PULLUP;
}

bool Input::IsKeyPushdown(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key] == KEY_STATE_PUSHDOWN;
}

bool Input::IsKeyOff(Uint8 SDL_SCANCODE_key)
{
	return mKey[SDL_SCANCODE_key] == KEY_STATE_OFF;
}

bool Input::IsKeyPressed(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key] == KEY_STATE_PRESSED;
}

bool Input::IsKeyPullup(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key] == KEY_STATE_PULLUP;
}

bool Input::IsKeyPushdown(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key] == KEY_STATE_PUSHDOWN;
}

bool Input::IsKeyOff(KEY_CONFIG key)
{
	return mKeyConfigedKeyState[key] == KEY_STATE_OFF;
}

InputController* Input::GetControllerInstance()
{
	return mController;
}

bool Input::ControllerInit()
{
	// コントローラ初期化
	mController = new InputController;
	return mController->Initialize();
}
