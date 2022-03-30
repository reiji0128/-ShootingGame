#pragma once
#include <SDL_mixer.h>
#include <unordered_map>    

class AudioManager
{
public:
	             AudioManager();
	            ~AudioManager();
	class Sound* GetSound(const std::string& fileName);
	void         RemoveSound(const std::string& fileName);
	void         PlaySound(const std::string& fileName);
	void         StopSound(const std::string& fileName);
	bool         IsPlayingSound(const std::string& fileName);

	class Music* GetMusic(const std::string& fileName);
	void         RemoveMusic(const std::string& fileName);
	void         PlayFadeInMusic(const std::string& fileName, int fadeInTime);
	void         PlayMusic(const std::string& fileName);
	void         FadeOutMusic(int fadeOutTime);
	void         StopMusic();
	bool         IsPlayingMusic();

	void         ShutDown();

private:
	std::unordered_map<std::string, class Sound*> mSounds;
	std::unordered_map<std::string, class Music*> mMusics;

};