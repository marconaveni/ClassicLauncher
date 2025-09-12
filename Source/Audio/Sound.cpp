#include "Audio/Sound.h"


#include "raylib.h"
#include "Sound.h"


namespace ClassicLauncher
{


    struct Sound::SoundData
    {
        ::Sound sound{};
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
        m_data->sound = ::LoadSound(path.string().c_str());
        m_data->isReady = ::IsSoundValid(m_data->sound);
        return m_data->isReady;
    }

    void Sound::Play()
    {
        ::PlaySound(m_data->sound);
    }

    void Sound::Stop()
    {
        ::StopSound(m_data->sound);
    }

    void Sound::Pause()
    {
        ::PauseSound(m_data->sound);
    }

    void Sound::Resume()
    {
        ::ResumeSound(m_data->sound);
    }

    void Sound::SetVolume(float volume)
    {
        ::SetSoundVolume(m_data->sound, volume);
    }

    bool Sound::IsPlaying() const
    {
        return ::IsSoundPlaying(m_data->sound);
    }

    bool Sound::IsValid() const
    {
        return m_data->isReady;
    }

    void Sound::Unload()
    {
        if (m_data->isReady)
        {
            ::UnloadSound(m_data->sound);
        }
    }

} // namespace Fusion::Audio
