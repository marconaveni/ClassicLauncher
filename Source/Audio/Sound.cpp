#include "Audio/Sound.h"

#include "Sound.h"
#include "Ray.h"

namespace ClassicLauncher
{


    struct Sound::SoundData
    {
        ray::Sound sound{};
        bool isReady = false;
    };


    Sound::Sound()
    {
        m_data = std::make_unique<SoundData>();
    }

    Sound::~Sound()
    {
        Unload();
    }

    bool Sound::LoadFromFile(const std::filesystem::path& path)
    {
        Unload();
        m_data->sound = ray::LoadSound(path.string().c_str());
        m_data->isReady = ray::IsSoundValid(m_data->sound);
        return m_data->isReady;
    }

    void Sound::Play()
    {
        ray::PlaySound(m_data->sound);
    }

    void Sound::Stop()
    {
        ray::StopSound(m_data->sound);
    }

    void Sound::Pause()
    {
        ray::PauseSound(m_data->sound);
    }

    void Sound::Resume()
    {
        ray::ResumeSound(m_data->sound);
    }

    void Sound::SetVolume(float volume)
    {
        ray::SetSoundVolume(m_data->sound, volume);
    }

    bool Sound::IsPlaying() const
    {
        return ray::IsSoundPlaying(m_data->sound);
    }

    bool Sound::IsValid() const
    {
        return m_data->isReady;
    }

    void Sound::Unload()
    {
        if (m_data->isReady)
        {
            ray::UnloadSound(m_data->sound);
        }
    }

} // namespace ClassicLauncher
