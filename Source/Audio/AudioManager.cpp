#include "AudioManager.h"

#include <chrono>
#include <filesystem>
#include <format>

#include "Utils/Math.h"
#include "Audio/Music.h"
#include "Audio/Sound.h"
#include "Audio/AudioDevice.h"


namespace ClassicLauncher
{

    AudioManager::AudioManager()
        : m_isRunning(false)
        , m_isPlayClick(false)
        , m_isPlayCursor(false)
        , m_statusAudio(Status::Stop)
        , m_clickSound{}
        , m_cursorSound{}
        , m_idAudioMusic(0)
    {
        m_clickSound = std::make_unique<Sound>();
        m_cursorSound = std::make_unique<Sound>();
    }

    void AudioManager::Init()
    {
        Unload();
        AudioDevice::GetInstance().Init();
            
        if (!m_isRunning)
        {
            m_isRunning = true;
            m_workerThread = std::thread(&AudioManager::Update, this);
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
        music->name = pat.stem().string();

        m_audioMusics.emplace_back(std::move(music));
    }

    void AudioManager::LoadMusics(const std::string& path, bool bAutoPlay)
    {
        std::filesystem::path directorypath = path;
        for (const auto& entry : std::filesystem::directory_iterator(directorypath)) 
        {
            LoadMusic(entry.path().string());
        }
        ChangeMusic(bAutoPlay);
    }

    void AudioManager::LoadCursor(const std::string& path)
    {
        m_cursorSound->LoadFromFile(path);
    }

    void AudioManager::LoadCLick(const std::string& path)
    {
        m_clickSound->LoadFromFile(path);
    }

    void AudioManager::Play()
    {
        if (!m_audioMusics.empty())
        {
            Music* currentMusic = m_audioMusics[m_idAudioMusic].get();
            if (m_statusAudio != Status::Playing)
            {
                currentMusic->Play();
                m_statusAudio = Status::Playing;
            }
        }
    }

    void AudioManager::PlayClick()
    {
        m_isPlayClick = true;
    }

    void AudioManager::PlayCursor()
    {
        m_isPlayCursor = true;
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

    std::string AudioManager::GetMusicName()
    {
        if (!m_audioMusics.empty())
        {
            return m_audioMusics[m_idAudioMusic]->name;
        }
        return {};
    }

    void AudioManager::ChangeMusic(bool bAutoPlay)
    {
        if (!m_audioMusics.empty())
        {
            Stop();
            m_idAudioMusic = (m_audioMusics.size() > 1) ? GenerateId() : 0;
            Music* currentMusic = m_audioMusics[m_idAudioMusic].get();
            currentMusic->Seek(0);
            if (bAutoPlay)
            {
                Play();
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


    void AudioManager::Update()
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
                
            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // wait
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
                m_workerThread.join();  // Espera a thread finalizar
            }
        }
    }

    AudioManager::~AudioManager()
    {
        Unload();
        const bool isReady = AudioDevice::GetInstance().IsReady();
        if (isReady)
        {
            AudioDevice::GetInstance().Shutdown();
        }
    }

}  // namespace ClassicLauncher