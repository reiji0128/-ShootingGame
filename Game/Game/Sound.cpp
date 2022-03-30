#include "Sound.h"

/// <summary>
/// コンストラクタ
/// </summary>
Sound::Sound()
    :mChunk(nullptr)
    ,mChannel(-1)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Sound::~Sound()
{
    Mix_FreeChunk(mChunk);
}

/// <summary>
/// 再生
/// </summary>
void Sound::Play()
{
    mChannel = Mix_PlayChannel(-1, mChunk, 0);
}

/// <summary>
/// 再生ストップ
/// </summary>
void Sound::Stop()
{
	if (mChannel == -1)
	{
		return;
	}

	if (Mix_Playing(mChannel))
	{
		Mix_HaltChannel(mChannel);
	}
	mChannel = -1;
}

/// <summary>
/// サウンドファイル読み込み
/// </summary>
/// <param name="fileName">ファイルパス</param>
/// <returns>
/// true  : 読み込み成功
/// false : 読み込み失敗
/// </returns>
bool Sound::LoadSound(const std::string& fileName)
{
	mChunk = Mix_LoadWAV(fileName.c_str());
	return mChunk != nullptr;
}

/// <summary>
/// 現在再生中か？
/// </summary>
/// <returns>
/// true  : 再生中
/// false : 再生していない
/// </returns>
bool Sound::IsPlaying()
{
	return mChannel >= 0 && Mix_Playing(mChannel);
}
