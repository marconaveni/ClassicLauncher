#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace ClassicLauncher
{

    class Music;
    class Sound;

    class AudioManager
    {
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
        void Unload();
        [[nodiscard]] bool IsPlayMusic() const { return (m_statusAudio == 1);}

    private:

        struct Status
        {
            inline static constexpr unsigned int Stop = 0;
            inline static constexpr unsigned int Playing = 1;
            inline static constexpr unsigned int Paused = 2;
        };

        std::mutex m_musicMutex;
        std::thread m_workerThread;        // Thread work
        std::atomic<bool> m_isRunning;     // Thread is Running
        std::atomic<bool> m_isPlayClick;   // Sinalize thread to play m_clickSound
        std::atomic<bool> m_isPlayCursor;  // Sinalize thread to play m_cursorSound
        std::atomic<int> m_statusAudio;    // Status Current Audio Music

        std::unique_ptr<Sound> m_clickSound;   //
        std::unique_ptr<Sound> m_cursorSound;  //

        std::vector<std::unique_ptr<Music>> m_audioMusics;  // Array Struct Audio musics
        int m_idAudioMusic{};                                 // id music

        void LoadMusic(const std::string& path);
        void Update();
        int GenerateId();
    };

}  // namespace ClassicLauncher

#endif
