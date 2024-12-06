#include "AudioManager.h"
#include "Utils/Core.h"
#include <chrono>


namespace ClassicLauncher
{


    AudioManager::AudioManager()
     : bIsRunning(false)
     , bIsPlayClick(false)
     , bIsPlayCursor(false)
     , status(StatusAudio::Stop)
     , clickMusic{ 0 }
     , cursorMusic{ 0 }
     , musics()
     , idMusic(0)
    {
    }

    void AudioManager::Init()
    {
        Unload();
        if (!bIsRunning) 
        {
            bIsRunning = true;
            workerThread = std::thread(&AudioManager::Update, this);
            // workerThread = std::thread([this]() {
            //     this->Update();
            // }); 
        }
    }

    void AudioManager::LoadMusic(const std::string& path)
    {
        AudioMusic audioMusic  { 0 } ;
        audioMusic.music = LoadMusicStream(path.c_str());

        if(!IsMusicValid(audioMusic.music))
        {
            return;
        }

        audioMusic.music.looping = false;
        audioMusic.name = GetFileNameWithoutExt(path.c_str());
        musics.emplace_back(audioMusic);
    }


    void AudioManager::LoadMusics(const std::string& path, bool bAutoPlay)
    {

        FilePathList files = LoadDirectoryFiles(path.c_str());
        for (unsigned int i = 0; i < files.count; i++)
        {
            LoadMusic(files.paths[i]);
        }
        UnloadDirectoryFiles(files);
        ChangeMusic(bAutoPlay);
    }

    void AudioManager::LoadCursor(const std::string& path)
    {
        cursorMusic = LoadSound(path.c_str());
    }

    void AudioManager::LoadCLick(const std::string& path)
    {
        clickMusic = LoadSound(path.c_str());
    }

    void AudioManager::Play()
    {
        if (!musics.empty())
        {
            Music& currentMusic = musics[idMusic].music; 
            if (status != StatusAudio::Playing)
            {
                PlayMusicStream(currentMusic);
                status = StatusAudio::Playing;
            }
        }
    }

    void AudioManager::PlayClick()
    {
        bIsPlayClick = true;
    }

    void AudioManager::PlayCursor()
    {
        bIsPlayCursor = true;
    }

    void AudioManager::Pause()
    {
        if (!musics.empty())
        {
            Music& currentMusic = musics[idMusic].music; 
            if (status == StatusAudio::Playing)
            {
                PauseMusicStream(currentMusic);
                status = StatusAudio::Paused;
            }
        }
    }

    void AudioManager::Stop()
    {
        if (!musics.empty())
        {
            Music& currentMusic = musics[idMusic].music; 
            status = StatusAudio::Stop;
            StopMusicStream(currentMusic);
        }
    }

    std::string AudioManager::GetMusicName()
    {
        if (!musics.empty()) 
        {
            return musics[idMusic].name;      
        }
        return std::string();
    }

    void AudioManager::ChangeMusic(bool bAutoPlay)
    {
        if (!musics.empty()) 
        {
            Stop();
            idMusic = (musics.size() > 1) ? GenerateId() : 0;
            const Music& currentMusic = musics[idMusic].music;
            SeekMusicStream(currentMusic, 0); 
            if(bAutoPlay)
            {
                Play();
            }
        }
    }

    int AudioManager::GenerateId()
    {
        int newId = idMusic;
        while (newId == idMusic)
        {
            newId = static_cast<int>(Math::Random(0.0f, static_cast<float>(musics.size()))) ;
        }
        
        return newId;  
    }

    void AudioManager::Stream(const Music& music)
    {
        if(IsMusicValid(music))
        {
            UpdateMusicStream(music);
        }
    }


    void AudioManager::Update()
    {
        while (bIsRunning) 
        {
            
            std::lock_guard<std::mutex> lock(musicMutex);

            if(!musics.empty() && status == StatusAudio::Playing)
            {
                Music& currentMusic = musics[idMusic].music; 
                Stream(currentMusic);
                if (GetMusicTimeLength(currentMusic) - 1 < GetMusicTimePlayed(currentMusic))
                {
                    ChangeMusic();
                }
            }
            if(IsSoundValid(clickMusic) && bIsPlayClick)
            {
                PlaySound(clickMusic);
                bIsPlayClick = !bIsPlayClick;
            }
            if(IsSoundValid(cursorMusic) && bIsPlayCursor)
            {
                PlaySound(cursorMusic);
                bIsPlayCursor = !bIsPlayCursor;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // wait
        }
    }

    void AudioManager::Unload()
    {
        if (bIsRunning) 
        {
            Stop();
            bIsRunning = false;

            for (auto &music : musics)
            {
                UnloadMusicStream(music.music);
            }
            musics.clear();
            if (workerThread.joinable()) 
            {
                workerThread.join(); // Espera a thread finalizar
            }
        }
    }

    AudioManager::~AudioManager()
    {
        Unload();
    }

}