#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <atomic>
#include <filesystem>
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
        void LoadSound(const std::filesystem::path& path, const std::string& name);
        void PlaySound(const std::string& name);
        void PlayMusic();
        void MusicVolume(float volume);
        void Pause();
        void Stop();
        void Update();
        std::string GetMusicName();
        void ChangeMusic(bool autoPlay = true);
        void Unload();
        [[nodiscard]] bool IsPlayMusic() const { return (m_statusAudio == 1); }

    private:

        struct Status
        {
            inline static constexpr unsigned int Stop = 0;
            inline static constexpr unsigned int Playing = 1;
            inline static constexpr unsigned int Paused = 2;
        };

        std::mutex m_musicMutex{};
        std::thread m_workerThread{};                 // Thread work
        std::atomic<bool> m_isRunning{false};         // Thread is Running
        std::atomic<bool> m_isPlayClick{false};       // Sinalize thread to play m_clickSound
        std::atomic<bool> m_isPlayCursor{false};      // Sinalize thread to play m_cursorSound
        std::atomic<int> m_statusAudio{Status::Stop}; // Status Current Audio Music

        std::unique_ptr<Sound> m_clickSound{nullptr};  //
        std::unique_ptr<Sound> m_cursorSound{nullptr}; //

        std::vector<std::unique_ptr<Music>> m_audioMusics{}; // Array Struct Audio musics
        int m_idAudioMusic{0};                               // id music

        void LoadMusic(const std::string& path);
        void UpdateStream();
        int GenerateId();
    };

} // namespace ClassicLauncher

#endif
