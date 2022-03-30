#pragma once

#include <SDL_mixer.h>
#include <iostream>

class Music
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Music();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Music();

	/// <summary>
	/// ミュージック再生
	/// </summary>
	void        Play();
	
	/// <summary>
	/// 再生ストップ
	/// </summary>
	static void Stop();

	/// <summary>
	/// ミュージックフェードイン
	/// </summary>
	/// <param name="fadeInTime">フェードインする時間</param>
	void        FadeInMusic(int fadeInTime);

	/// <summary>
	/// ミュージックフェードアウト
	/// </summary>
	/// <param name="fadeOutTime">フェードアウトする時間</param>
	static void FadeOutMusic(int fadeOutTime);

	/// <summary>
	/// ミュージック読み込み
	/// </summary>
	/// <param name="fileName">ファイルパス</param>
	/// <returns>
	/// true  : 読み込み成功
	/// false : 読み込み失敗
	/// </returns>
	bool        LoadMusic(const std::string& fileName);

	/// <summary>
	/// 現在ミュージックは再生中か？
	/// </summary>
	/// <returns>
	/// true  : 再生中
	/// false : 再生していない
	/// </returns>
	static bool IsPlaying();

private:
	// 音源データ
	Mix_Music* mMusic;
};