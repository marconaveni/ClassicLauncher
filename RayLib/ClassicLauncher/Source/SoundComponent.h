#pragma once

#include "raylib.h"
#include <vector>
#include <string>


enum StateSound 
{
    Stop,
    Playing,
    Pause,
    Ended
};

class SoundComponent
{

    SoundComponent()
        : currentMusic(),
        click(),
        cursor(),
        volumeMusic(0.8f),
        volumeClick(0.8f),
        volumeCursor(0.8f)
    {
    }

    std::vector<std::string> musics;

    Music currentMusic;

    Sound click;

    Sound cursor;

    float volumeMusic;

    float volumeClick;

    float volumeCursor;

    StateSound stateSound = StateSound::Stop;


public:


    static SoundComponent* GetInstance();

    void LoadUiSounds(const char* clickPath, const char* cursorPath);

    void LoadCurrentMusic(const char* path);

    void LoadPathMusics(const char* path);

    void BeginPlay();

    void Tick();

    void EndPlay();

    void PlayMusic();

    void PlayClick();

    void PlayCursor();

    void PauseMusic();

    void StopMusic();

    void SeekMusic(float newLength);

    float GetMusicLength() const;

    float GetMusicPlayed() const;

    void SetMusicVolume(float volume);

    void SetClickVolume(float volume);

    void SetCursorVolume(float volume);

private:

    void ClearMusics();
    
    bool IsMusicPlaying() const;
};
