#ifndef SOUND_H
#define SOUND_H

#include <filesystem>
#include <memory>

namespace ClassicLauncher
{
    class Sound
    {
    public:

        Sound();
        ~Sound();

        // Desabilita cópia para gerenciar o ponteiro de forma segura
        Sound(const Sound&) = delete;
        Sound& operator=(const Sound&) = delete;

        bool LoadFromFile(const std::filesystem::path& path);

        void Play();
        void Stop();
        void Pause();
        void Resume();

        void SetVolume(float volume); // Volume de 0.0f a 1.0f
        bool IsPlaying() const;
        bool IsValid() const;

    private:

        struct SoundData; // PIMPL Idiom https://www.geeksforgeeks.org/cpp/pimpl-idiom-in-c-with-examples/
        std::unique_ptr<SoundData> m_data;
        void Unload();
    };
} // namespace ClassicLauncher

#endif // SOUND_H