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

    enum StatusAudio
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

        std::mutex musicMutex;
        std::thread workerThread;         // Thread work
        std::atomic<bool> bIsRunning;     // Thread is Running 
        std::atomic<bool> bIsPlayClick;   // Sinaliza para a thread tocar clickMusic
        std::atomic<bool> bIsPlayCursor;  // Sinaliza para a thread tocar cursorMusic
        std::atomic<StatusAudio> status;  // Status Current Audio Music
        Sound clickMusic;                 // Struct Sound
        Sound cursorMusic;                // Struct Sound
        std::vector<AudioMusic> musics;   // Array Struct Audio musics
        int idMusic;                      // id music

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
