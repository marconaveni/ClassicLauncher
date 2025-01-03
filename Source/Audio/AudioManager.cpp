#include "AudioManager.h"
#include <chrono>
#include "Core.h"

namespace ClassicLauncher
{

    AudioManager::AudioManager()
        : mIsRunning(false), mIsPlayClick(false), mIsPlayCursor(false), mStatusAudio(StatusAudioMusic::Stop), mClickSound{}, mCursorSound{}, mAudioMusics(), mIdAudioMusic(0)
    {
    }

    void AudioManager::Init()
    {
        Unload();
        if (!mIsRunning)
        {
            mIsRunning = true;
            mWorkerThread = std::thread(&AudioManager::Update, this);
        }
    }

    void AudioManager::LoadMusic(const std::string& path)
    {
        AudioMusic audioMusic{};
        audioMusic.music = LoadMusicStream(path.c_str());

        if (!IsMusicValid(audioMusic.music))
        {
            return;
        }

        audioMusic.music.looping = false;
        audioMusic.name = GetFileNameWithoutExt(path.c_str());
        mAudioMusics.emplace_back(audioMusic);
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
        mCursorSound = LoadSound(path.c_str());
    }

    void AudioManager::LoadCLick(const std::string& path)
    {
        mClickSound = LoadSound(path.c_str());
    }

    void AudioManager::Play()
    {
        if (!mAudioMusics.empty())
        {
            Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
            if (mStatusAudio != StatusAudioMusic::Playing)
            {
                PlayMusicStream(currentMusic);
                mStatusAudio = StatusAudioMusic::Playing;
            }
        }
    }

    void AudioManager::PlayClick()
    {
        mIsPlayClick = true;
    }

    void AudioManager::PlayCursor()
    {
        mIsPlayCursor = true;
    }

    void AudioManager::Pause()
    {
        if (!mAudioMusics.empty())
        {
            Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
            if (mStatusAudio == StatusAudioMusic::Playing)
            {
                PauseMusicStream(currentMusic);
                mStatusAudio = StatusAudioMusic::Paused;
            }
        }
    }

    void AudioManager::Stop()
    {
        if (!mAudioMusics.empty())
        {
            Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
            mStatusAudio = StatusAudioMusic::Stop;
            StopMusicStream(currentMusic);
        }
    }

    std::string AudioManager::GetMusicName()
    {
        if (!mAudioMusics.empty())
        {
            return mAudioMusics[mIdAudioMusic].name;
        }
        return std::string();
    }

    void AudioManager::ChangeMusic(bool bAutoPlay)
    {
        if (!mAudioMusics.empty())
        {
            Stop();
            mIdAudioMusic = (mAudioMusics.size() > 1) ? GenerateId() : 0;
            const Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
            SeekMusicStream(currentMusic, 0);
            if (bAutoPlay)
            {
                Play();
            }
        }
    }

    int AudioManager::GenerateId()
    {
        int newId = mIdAudioMusic;
        while (newId == mIdAudioMusic)
        {
            newId = static_cast<int>(Math::Random(0.0f, static_cast<float>(mAudioMusics.size())));
        }

        return newId;
    }

    void AudioManager::Stream(const Music& music)
    {
        if (IsMusicValid(music))
        {
            UpdateMusicStream(music);
        }
    }

    void AudioManager::Update()
    {
        while (mIsRunning)
        {
            std::lock_guard<std::mutex> lock(mMusicMutex);

            if (!mAudioMusics.empty() && mStatusAudio == StatusAudioMusic::Playing)
            {
                Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
                Stream(currentMusic);
                if (GetMusicTimeLength(currentMusic) - 1 < GetMusicTimePlayed(currentMusic))
                {
                    ChangeMusic();
                }
            }
            if (IsSoundValid(mClickSound) && mIsPlayClick)
            {
                PlaySound(mClickSound);
                mIsPlayClick = !mIsPlayClick;
            }
            if (IsSoundValid(mCursorSound) && mIsPlayCursor)
            {
                PlaySound(mCursorSound);
                mIsPlayCursor = !mIsPlayCursor;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // wait
        }
    }

    void AudioManager::Unload()
    {
        if (mIsRunning)
        {
            Stop();
            mIsRunning = false;

            for (auto& music : mAudioMusics)
            {
                UnloadMusicStream(music.music);
            }
            mAudioMusics.clear();
            if (mWorkerThread.joinable())
            {
                mWorkerThread.join();  // Espera a thread finalizar
            }
        }
    }

    AudioManager::~AudioManager()
    {
        Unload();
    }

}  // namespace ClassicLauncher