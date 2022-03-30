#pragma once

#include <SDL_mixer.h>
#include <iostream>

class Sound
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sound();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sound();

	/// <summary>
	/// 再生
	/// </summary>
	void          Play();

	/// <summary>
	/// 再生ストップ
	/// </summary>
	void          Stop();

	/// <summary>
	/// サウンドファイル読み込み
	/// </summary>
	/// <param name="fileName">ファイルパス</param>
	/// <returns>
	/// true  : 読み込み成功
	/// false : 読み込み失敗
	/// </returns>
	bool          LoadSound(const std::string& fileName);

	/// <summary>
	/// 現在再生中か？
	/// </summary>
	/// <returns>
	/// true  : 再生中
	/// false : 再生していない
	/// </returns>
	bool          IsPlaying();

private:
	// サウンドデータ
	Mix_Chunk* mChunk;

	// 再生チャンネル
	int        mChannel;
};