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
        std::thread workerThread;         // Thread para o trabalho
        std::atomic<bool> bIsRunning;     // Sinaliza se a thread deve continuar
        std::atomic<bool> bIsPlayClick;   // Sinaliza se a thread deve continuar
        std::atomic<bool> bIsPlayCursor;  // Sinaliza se a thread deve continuar
        std::atomic<StatusAudio> status;  // Sinaliza se a thread deve continuar
        Sound clickMusic;                 // Struct music
        Sound cursorMusic;                // Struct music
        std::vector<AudioMusic> musics;   // Struct music
        int idMusic;                      // Struct music

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
