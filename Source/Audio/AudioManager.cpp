#include "AudioManager.h"
#include <chrono>
#include "Utils/Math.h"

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
        audioMusic.music = rlw::LoadMusicStream(path.c_str());

        if (!IsMusicValid(audioMusic.music))
        {
            return;
        }

        audioMusic.music.looping = false;
        audioMusic.name = rlw::GetFileNameWithoutExt(path.c_str());
        mAudioMusics.emplace_back(audioMusic);
    }

    void AudioManager::LoadMusics(const std::string& path, bool bAutoPlay)
    {
        rlw::FilePathList files = rlw::LoadDirectoryFiles(path.c_str());
        for (unsigned int i = 0; i < files.count; i++)
        {
            LoadMusic(files.paths[i]);
        }
        UnloadDirectoryFiles(files);
        ChangeMusic(bAutoPlay);
    }

    void AudioManager::LoadCursor(const std::string& path)
    {
        mCursorSound = rlw::LoadSound(path.c_str());
    }

    void AudioManager::LoadCLick(const std::string& path)
    {
        mClickSound = rlw::LoadSound(path.c_str());
    }

    void AudioManager::Play()
    {
        if (!mAudioMusics.empty())
        {
            rlw::Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
            if (mStatusAudio != StatusAudioMusic::Playing)
            {
                rlw::PlayMusicStream(currentMusic);
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
            rlw::Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
            if (mStatusAudio == StatusAudioMusic::Playing)
            {
                rlw::PauseMusicStream(currentMusic);
                mStatusAudio = StatusAudioMusic::Paused;
            }
        }
    }

    void AudioManager::Stop()
    {
        if (!mAudioMusics.empty())
        {
            rlw::Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
            mStatusAudio = StatusAudioMusic::Stop;
            rlw::StopMusicStream(currentMusic);
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
            const rlw::Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
            rlw::SeekMusicStream(currentMusic, 0);
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

    void AudioManager::Stream(const rlw::Music& music)
    {
        if (rlw::IsMusicValid(music))
        {
            rlw::UpdateMusicStream(music);
        }
    }

    void AudioManager::Update()
    {
        while (mIsRunning)
        {
            std::lock_guard<std::mutex> lock(mMusicMutex);

            if (!mAudioMusics.empty() && mStatusAudio == StatusAudioMusic::Playing)
            {
                rlw::Music& currentMusic = mAudioMusics[mIdAudioMusic].music;
                Stream(currentMusic);
                if (rlw::GetMusicTimeLength(currentMusic) - 1 < rlw::GetMusicTimePlayed(currentMusic))
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