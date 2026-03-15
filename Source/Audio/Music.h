#ifndef MUSIC_H
#define MUSIC_H

#include <filesystem>
#include <memory>
#include <string>

namespace ClassicLauncher
{
    
    class Music
    {
    public:

        Music();
        ~Music();

        Music(const Music&) = delete;
        Music& operator=(const Music&) = delete;

        bool LoadFromFile(const std::filesystem::path& path);

        void Play();
        void Stop();
        void Pause();
        void Resume();
        void UpdateStream();

        void SetVolume(float volume);
        bool IsPlaying() const;
        bool IsValid() const;
        float GetDuration() const;
        float GetTimePlayed() const;
        void SetLoop(bool enable);
        void Seek(float time);
        std::string m_name{};

    private:

        struct MusicData; // PIMPL Idiom https://www.geeksforgeeks.org/cpp/pimpl-idiom-in-c-with-examples/
        std::unique_ptr<MusicData> m_data{};
        void Unload();
    };
} // namespace ClassicLauncher

#endif // MUSIC_H