#include "Sprite.h"

#include <atomic>
#include <string>
#include <thread>

#include "ClassicAssert.h"
#include "Utils/Log.h"
#include "Utils/UtilsFunctionLibrary.h"

namespace ClassicLauncher
{

    Sprite::Sprite()
        : mIsKeepRunning(false), mIsImageLoaded(false), mIsTextureLoaded(false), mImage(), mTexture()
    {
    }

    Sprite::~Sprite()
    {
        Stop();
        Join();
        Unload();
        LOG(LOG_CLASSIC_TRACE, "Sprite - thread stopped and class destroyed");
    }

    void Sprite::Load(const std::string& file, const int width, const int height, bool bAspectRatio)
    {
        if (!mIsKeepRunning && !mIsTextureLoaded && !mIsImageLoaded)
        {
            Join();
            mIsKeepRunning = true;
            mFilePath = file;
            LOG(LOG_CLASSIC_TRACE, "Sprite - starting thread");
            mWorkerThread = std::thread(&Sprite::LoadImage, this, width, height, bAspectRatio);
        }
    }

    void Sprite::Load(Image& newImage, const int width, const int height, const bool bAspectRatio)
    {
        if (newImage.IsValid())
        {
            Unload();
            newImage.CopyTo(mImage);
            mFilePath = "[loaded from memory]";
            ResizeImage(width, height, bAspectRatio);
            mIsImageLoaded = mImage.IsValid();
            LOG(LOG_CLASSIC_TRACE, "Image copied successfully");
        }
    }

    void Sprite::Stop()
    {
        mIsKeepRunning = false; // Sinaliza para encerrar
    }

    void Sprite::Join()
    {
        if (mWorkerThread.joinable())
        {
            mWorkerThread.join();
        }
    }

    void Sprite::LoadImage(const int width, const int height, bool bAspectRatio)
    {
        // std::this_thread::sleep_for(std::chrono::seconds(1)); //for test
        if (mIsKeepRunning)
        {
            // std::this_thread::sleep_for(std::chrono::seconds(1)); //for test
            mImage.LoadFromFile(mFilePath);
            if (mImage.IsValid())
            {
                ResizeImage(width, height, bAspectRatio);
                mIsImageLoaded = mImage.IsValid();
                LOG(LOG_CLASSIC_TRACE, "Image loaded successfully from - \"%s\"", mFilePath.c_str());
            }
            else
            {
                LOG(LOG_CLASSIC_WARNING, "Failed to load Image - \"%s\"", mFilePath.c_str());
            }
        }
        LOG(LOG_CLASSIC_TRACE, "LoadImage - finished");
        Stop();
    }

    Texture* Sprite::GetTexture()
    {
        if (!mIsTextureLoaded && mIsImageLoaded)
        {
            // mTexture = rlw::LoadTextureFromImage(mImage);
            mTexture.LoadFromImage(&mImage);
            mIsTextureLoaded = mTexture.IsValid();
            LOG(LOG_CLASSIC_TRACE, "Texture loaded [ID %d] from Image - \"%s\"", mTexture.GetId(), mFilePath.c_str());
            UnloadImage();
        }
        if (mIsTextureLoaded)
        {
            return &mTexture;
        }
        return nullptr;
    }

    Image* Sprite::GetImage()
    {
        if (mIsImageLoaded)
        {
            return &mImage;
        }
        return nullptr;
    }

    void Sprite::ResizeImage(const int width, const int height, bool bAspectRatio)
    {
        std::lock_guard<std::mutex> guard(mMutexSprite);
        if (width > 0 && height > 0 && mImage.IsValid())
        {
            if (bAspectRatio)
            {
                Utils::ImageResize(mImage, width, height);
            }
            else
            {
                //rlw::ImageResize(&mImage, width, height);
                mImage.Resize(width, height);
            }
            if (mIsTextureLoaded)
            {
                mTexture.Update(mImage.data);
            }
        }
    }

    void Sprite::Unload()
    {
        UnloadImage();
        UnloadTexture();
    }

    void Sprite::UnloadTexture()
    {
        if (mIsTextureLoaded && mTexture.IsValid())
        {
            mTexture.Unload();
            LOG(LOG_CLASSIC_TRACE, "Unloaded Texture [ID %d] from - \"%s\"", mTexture.GetId(), mFilePath.c_str());
            mIsTextureLoaded = false;
        }
    }

    void Sprite::UnloadImage()
    {
        if (mIsImageLoaded && mImage.IsValid())
        {
            mImage.Unload();
            LOG(LOG_CLASSIC_TRACE, "Unloaded Image from - %s", mFilePath.c_str());
            mImage = {};
            mIsImageLoaded = false;
        }
    }

} // namespace ClassicLauncher
