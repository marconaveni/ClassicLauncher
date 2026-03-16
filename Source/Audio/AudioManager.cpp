#include "AudioManager.h"

#include <chrono>
#include <filesystem>

#include "Audio/AudioDevice.h"
#include "Helper.h"
#include "Input/InputManager.h"
#include "Utils/Log.h"
#include "Utils/Math.h"


namespace ClassicLauncher
{

    AudioManager::AudioManager()
    {
        m_clickSound = std::make_unique<Sound>();
        m_cursorSound = std::make_unique<Sound>();
    }

    void AudioManager::Init()
    {
        const bool isReady = AudioDevice::GetInstance().IsReady();
        if (!isReady)
        {
            AudioDevice::GetInstance().Init();
        }

        if (!m_isRunning)
        {
            m_isRunning = true;
            m_workerThread = std::thread(&AudioManager::UpdateStream, this);
        }
    }

    void AudioManager::LoadMusic(const std::string& path)
    {

        auto music = std::make_unique<Music>();
        music->LoadFromFile(path.c_str());

        if (!music->IsValid())
        {
            return;
        }

        music->SetLoop(false);

        std::filesystem::path pat = path;
        music->m_name = pat.stem().string();

        m_audioMusics.emplace_back(std::move(music));
    }

    void AudioManager::LoadMusics(const std::string& path, bool isAutoPlay)
    {
        std::filesystem::path directorypath = path;
        if (!std::filesystem::exists(directorypath))
        {
            LOG(LogClassicError, "%s is not exists", directorypath.c_str());
            return;
        }

        for (const auto& entry : std::filesystem::directory_iterator(directorypath))
        {
            LoadMusic(entry.path().string());
        }
        ChangeMusic(isAutoPlay);
    }

    void AudioManager::LoadSound(const std::filesystem::path& path, const std::string& name)
    {
        if (!std::filesystem::exists(path))
        {
            return;
        }

        if (name == "click")
        {
            m_clickSound->LoadFromFile(path);
        }
        if (name == "cursor")
        {
            m_cursorSound->LoadFromFile(path);
        }
    }

    void AudioManager::PlaySound(const std::string& name)
    {
        m_isPlayClick = (name == "click");
        m_isPlayCursor = (name == "cursor");
    }

    void AudioManager::PlayMusic()
    {
        if (!m_audioMusics.empty())
        {
            Music* currentMusic = m_audioMusics[m_idAudioMusic].get();
            if (m_statusAudio != Status::Playing)
            {
                currentMusic->Play();
                currentMusic->SetVolume(m_musicVolume);
                m_statusAudio = Status::Playing;
            }
        }
    }

    void AudioManager::MusicVolume(float volume)
    {
        if (!m_audioMusics.empty())
        {
            Music* currentMusic = m_audioMusics[m_idAudioMusic].get();
            if (m_statusAudio == Status::Playing)
            {
                m_musicVolume = volume;
                currentMusic->SetVolume(volume);
            }
        }
    }

    void AudioManager::Pause()
    {
        if (!m_audioMusics.empty())
        {
            Music* currentMusic = m_audioMusics[m_idAudioMusic].get();
            if (m_statusAudio == Status::Playing)
            {
                currentMusic->Pause();
                m_statusAudio = Status::Paused;
            }
        }
    }

    void AudioManager::Stop()
    {
        if (!m_audioMusics.empty())
        {
            Music* currentMusic = m_audioMusics[m_idAudioMusic].get();
            m_statusAudio = Status::Stop;
            currentMusic->Stop();
        }
    }

    void AudioManager::Update()
    {
        if (InputManager::IsRelease(InputName::R3, MainCenter))
        {
            ChangeMusic();
            PRINT(TEXT("Changed music"), 5.0f);
            // todo add callback function to gui layout in release version
        }

        if (Keyboard::IsReleased(Keyboard::P) && IsPlayMusic())
        {
            Pause();
            PRINT(TEXT("Pause music"), 5.0f);
        }
        else if (Keyboard::IsReleased(Keyboard::P))
        {
            PlayMusic();
            PRINT(TEXT("Play music"), 5.0f);
        }
    }

    std::string AudioManager::GetMusicName()
    {
        if (!m_audioMusics.empty())
        {
            return m_audioMusics[m_idAudioMusic]->m_name;
        }
        return {};
    }

    void AudioManager::ChangeMusic(bool autoPlay)
    {
        if (!m_audioMusics.empty())
        {
            Stop();
            m_idAudioMusic = (m_audioMusics.size() > 1) ? GenerateId() : 0;
            Music* currentMusic = m_audioMusics[m_idAudioMusic].get();
            currentMusic->Seek(0);
            if (autoPlay)
            {
                PlayMusic();
            }
        }
    }

    int AudioManager::GenerateId()
    {
        int newId = m_idAudioMusic;
        while (newId == m_idAudioMusic)
        {
            newId = static_cast<int>(Math::Random(0.0f, static_cast<float>(m_audioMusics.size())));
        }

        return newId;
    }


    void AudioManager::UpdateStream()
    {
        while (m_isRunning)
        {
            std::lock_guard<std::mutex> lock(m_musicMutex);

            if (!m_audioMusics.empty() && m_statusAudio == Status::Playing)
            {
                Music* currentMusic = m_audioMusics[m_idAudioMusic].get();
                currentMusic->UpdateStream();
                if (currentMusic->GetDuration() - 1 < currentMusic->GetTimePlayed())
                {
                    ChangeMusic();
                }
            }

            if (m_isPlayClick || m_isPlayCursor)
            {
                if (m_clickSound->IsValid() && m_isPlayClick)
                {
                    m_clickSound->Play();
                    m_isPlayClick = !m_isPlayClick;
                }
                if (m_cursorSound->IsValid() && m_isPlayCursor)
                {
                    m_cursorSound->Play();
                    m_isPlayCursor = !m_isPlayCursor;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // wait
        }
    }

    void AudioManager::Unload()
    {
        if (m_isRunning)
        {
            Stop();
            m_isRunning = false;
            m_audioMusics.clear();
            if (m_workerThread.joinable())
            {
                m_workerThread.join(); // Espera a thread finalizar
            }
        }
        const bool isReady = AudioDevice::GetInstance().IsReady();
        if (isReady)
        {
            AudioDevice::GetInstance().Shutdown();
        }
    }

    AudioManager::~AudioManager()
    {
        Unload();
    }

} // namespace ClassicLauncher