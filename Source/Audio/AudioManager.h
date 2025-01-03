#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "raylib.h"

namespace ClassicLauncher
{

    enum StatusAudioMusic
    {
        Stop,
        Playing,
        Paused
    };

    struct AudioMusic
    {
        Music music;
        std::string name;
    };

    class AudioManager
    {
    private:

        std::mutex mMusicMutex;
        std::thread mWorkerThread;                   // Thread work
        std::atomic<bool> mIsRunning;                // Thread is Running
        std::atomic<bool> mIsPlayClick;              // Sinalize thread to play mClickSound
        std::atomic<bool> mIsPlayCursor;             // Sinalize thread to play mCursorSound
        std::atomic<StatusAudioMusic> mStatusAudio;  // Status Current Audio Music
        Sound mClickSound;                           // Struct Sound
        Sound mCursorSound;                          // Struct Sound
        std::vector<AudioMusic> mAudioMusics;        // Array Struct Audio musics
        int mIdAudioMusic;                           // id music

    public:

        AudioManager();
        ~AudioManager();

        void Init();
        void LoadMusics(const std::string& path, bool bAutoPlay = true);
        void LoadCursor(const std::string& path);
        void LoadCLick(const std::string& path);
        void Play();
        void PlayClick();
        void PlayCursor();
        void Pause();
        void Stop();
        std::string GetMusicName();
        void ChangeMusic(bool bAutoPlay = true);
        StatusAudioMusic GetStatusAudioMusic() { return mStatusAudio; }

    private:

        void LoadMusic(const std::string& path);
        void Stream(const Music& music);
        void Update();
        int GenerateId();

    public:

        void Unload();
    };

}  // namespace ClassicLauncher

#endif
