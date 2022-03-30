#include "Music.h"

/// <summary>
/// コンストラクタ
/// </summary>
Music::Music()
    :mMusic(nullptr) 
{
}

/// <summary>
/// デストラクタ
/// </summary>
Music::~Music()
{
}

/// <summary>
/// ミュージック再生
/// </summary>
void Music::Play()
{
    if (mMusic)
    {
        Mix_PlayMusic(mMusic, -1); // -1はループ再生設定
    }
}

/// <summary>
/// 再生ストップ
/// </summary>
void Music::Stop()
{
    Mix_HaltMusic();
}

/// <summary>
/// ミュージックフェードイン
/// </summary>
/// <param name="fadeInTime">フェードインする時間</param>
void Music::FadeInMusic(int fadeInTime)
{
    if(mMusic)
    {
        Mix_FadeInMusic(mMusic, -1, fadeInTime);
    }
}

/// <summary>
/// ミュージックフェードアウト
/// </summary>
/// <param name="fadeOutTime">フェードアウトする時間</param>
void Music::FadeOutMusic(int fadeOutTime)
{
    Mix_FadeOutMusic(fadeOutTime);
}

/// <summary>
/// ミュージック読み込み
/// </summary>
/// <param name="fileName"></param>
/// <returns></returns>
bool Music::LoadMusic(const std::string& fileName)
{
    mMusic = Mix_LoadMUS(fileName.c_str());
    return mMusic != nullptr;
}

/// <summary>
/// 現在ミュージックは再生中か？
/// </summary>
/// <returns></returns>
bool Music::IsPlaying()
{
    return Mix_PlayingMusic();
}
