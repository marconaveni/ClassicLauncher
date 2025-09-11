#ifndef MUSIC_H
#define MUSIC_H

#include <memory>
#include <string_view>
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

        bool LoadFromFile(std::string_view path);

        void Play();
        void Stop();
        void Pause();
        void Resume();
        void UpdateStream(); // Essencial para streaming!

        void SetVolume(float volume);
        bool IsPlaying() const;
        bool IsValid() const;
        float GetDuration() const;
        float GetTimePlayed() const;
        void SetLoop(bool enable);
        void Seek(float time);
        std::string name{};

    private:
        struct MusicData; // PIMPL Idiom https://www.geeksforgeeks.org/cpp/pimpl-idiom-in-c-with-examples/
        std::unique_ptr<MusicData> m_data;
        void Unload();
    };
}

#endif // MUSIC_H