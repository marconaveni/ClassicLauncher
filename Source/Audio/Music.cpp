#include "Audio/Music.h"

#include "Music.h"
#include "Ray.h"


namespace ClassicLauncher
{

    struct Music::MusicData
    {
        ray::Music musicStream{};
        bool isReady = false;
    };


    Music::Music()
    {
        m_data = std::make_unique<MusicData>();
    }

    Music::~Music()
    {
        Unload();
    }

    bool Music::LoadFromFile(const std::filesystem::path& path)
    {
        Unload();
        m_data->musicStream = ray::LoadMusicStream(path.string().c_str());
        m_data->isReady = ray::IsMusicValid(m_data->musicStream);
        return m_data->isReady;
    }

    void Music::Play()
    {
        ray::PlayMusicStream(m_data->musicStream);
    }

    void Music::Stop()
    {
        ray::StopMusicStream(m_data->musicStream);
    }

    void Music::Pause()
    {
        ray::PauseMusicStream(m_data->musicStream);
    }

    void Music::Resume()
    {
        ray::ResumeMusicStream(m_data->musicStream);
    }

    void Music::UpdateStream()
    {
        ray::UpdateMusicStream(m_data->musicStream);
    }

    void Music::SetVolume(float volume)
    {
        ray::SetMusicVolume(m_data->musicStream, volume);
    }

    bool Music::IsPlaying() const
    {
        return ray::IsMusicStreamPlaying(m_data->musicStream);
    }

    bool Music::IsValid() const
    {
        return ray::IsMusicValid(m_data->musicStream);
    }

    float Music::GetDuration() const
    {
        return ray::GetMusicTimeLength(m_data->musicStream);
    }

    float Music::GetTimePlayed() const
    {
        return ray::GetMusicTimePlayed(m_data->musicStream);
    }

    void Music::SetLoop(bool enable)
    {
        m_data->musicStream.looping = enable;
    }

    void Music::Seek(float time)
    {
        ray::SeekMusicStream(m_data->musicStream, time);
    }

    void Music::Unload()
    {
        if (m_data->isReady)
        {
            ray::UnloadMusicStream(m_data->musicStream);
        }
    }

} // namespace ClassicLauncher
