#include "Sprite.h"
#include <atomic>
#include <string>
#include <thread>
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

    void Sprite::Load(const Image& newImage, const int width, const int height, const bool bAspectRatio)
    {
        if (rlw::IsImageValid(newImage))
        {
            Unload();
            mImage = rlw::ImageCopy(newImage);
            mFilePath = "[loaded from memory]";
            ResizeImage(width, height, bAspectRatio);
            mIsImageLoaded = rlw::IsImageValid(mImage);
            LOG(LOG_CLASSIC_TRACE, "Image copied successfully");
        }
    }

    void Sprite::Stop()
    {
        mIsKeepRunning = false;  // Sinaliza para encerrar
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
            mImage = rlw::LoadImage(mFilePath.c_str());
            if (rlw::IsImageValid(mImage))
            {
                ResizeImage(width, height, bAspectRatio);
                mIsImageLoaded = rlw::IsImageValid(mImage);
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

    rlw::Texture2D* Sprite::GetTexture()
    {
        if (!mIsTextureLoaded && mIsImageLoaded)
        {
            mTexture = rlw::LoadTextureFromImage(mImage);
            mIsTextureLoaded = rlw::IsTextureValid(mTexture);
            LOG(LOG_CLASSIC_TRACE, "Texture loaded [ID %d] from Image - \"%s\"", mTexture.id, mFilePath.c_str());
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
        if (width > 0 && height > 0 && rlw::IsImageValid(mImage))
        {
            if (bAspectRatio)
            {
                UtilsFunctionLibrary::ImageResize(mImage, width, height);
            }
            else
            {
                rlw::ImageResize(&mImage, width, height);
            }
            if (mIsTextureLoaded)
            {
                UpdateTexture(mTexture, mImage.data);
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
        if (mIsTextureLoaded && rlw::IsTextureValid(mTexture))
        {
            rlw::UnloadTexture(mTexture);
            LOG(LOG_CLASSIC_TRACE, "Unloaded Texture [ID %d] from - \"%s\"", mTexture.id, mFilePath.c_str());
            mTexture = {};
            mIsTextureLoaded = false;
        }
    }

    void Sprite::UnloadImage()
    {
        if (mIsImageLoaded && rlw::IsImageValid(mImage))
        {
            rlw::UnloadImage(mImage);
            LOG(LOG_CLASSIC_TRACE, "Unloaded Image from - %s", mFilePath.c_str());
            mImage = {};
            mIsImageLoaded = false;
        }
    }

}  // namespace ClassicLauncher
