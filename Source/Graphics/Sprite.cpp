#include "Sprite.h"
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

namespace ClassicLauncher
{

    Sprite::Sprite()
        : bKeepRunning(false), bImageLoaded(false), bTextureLoaded(false), workerThread(), image(), texture(), filePath()
    {
    }

    Sprite::~Sprite()
    {
        TraceLog(LOG_TRACE, "Sprite - stopping thread");
        Stop();
        Join();
        Unload();
        TraceLog(LOG_TRACE, "Sprite - thread stopped");
    }

    void Sprite::Load(const std::string& file, const int width, const int height, bool bAspectRatio)
    {
        if (!bKeepRunning && !bTextureLoaded && !bImageLoaded)
        {
            Join();
            bKeepRunning = true;
            filePath = file;
            TraceLog(LOG_TRACE, "Sprite - starting thread");
            workerThread = std::thread(&Sprite::LoadImage, this, width, height, bAspectRatio);
        }
    }

    void Sprite::Load(const Image& newImage, const int width, const int height, const bool bAspectRatio)
    {
        if (IsImageValid(newImage))
        {
            Unload();
            image = ImageCopy(newImage);
            filePath = "[loaded from memory]";
            ResizeImage(width, height, bAspectRatio);
            bImageLoaded = IsImageValid(image);
            TraceLog(LOG_DEBUG, "Image copied successfully");
        }
    }

    void Sprite::Stop()
    {
        bKeepRunning = false;  // Sinaliza para encerrar
    }

    void Sprite::Join()
    {
        if (workerThread.joinable())
        {
            workerThread.join();
        }
    }

    void Sprite::LoadImage(const int width, const int height, bool bAspectRatio)
    {
        TraceLog(LOG_TRACE, "LoadImage - started ");
        // std::this_thread::sleep_for(std::chrono::seconds(1)); //for test
        if (bKeepRunning)
        {
            // std::this_thread::sleep_for(std::chrono::seconds(1)); //for test
            image = ::LoadImage(filePath.c_str());
            if (IsImageValid(image))
            {
                ResizeImage(width, height, bAspectRatio);
                bImageLoaded = IsImageValid(image);
                TraceLog(LOG_DEBUG, "Image loaded successfully from - %s", filePath.c_str());
            }
            else
            {
                TraceLog(LOG_WARNING, "Failed to load Image - %s", filePath.c_str());
            }
        }
        TraceLog(LOG_TRACE, "LoadImage - finished");
        Stop();
    }

    Texture2D* Sprite::GetTexture()
    {
        if (!bTextureLoaded && bImageLoaded)
        {
            texture = ::LoadTextureFromImage(image);
            bTextureLoaded = IsTextureValid(texture);
            TraceLog(LOG_DEBUG, "Texture loaded [ID %d] from Image - %s", texture.id, filePath.c_str());
            UnloadImage();
        }
        if (bTextureLoaded)
        {
            return &texture;
        }
        return nullptr;
    }

    Image* Sprite::GetImage()
    {
        if (bImageLoaded)
        {
            return &image;
        }
        return nullptr;
    }

    void Sprite::ResizeImage(const int width, const int height, bool bAspectRatio)
    {
        std::lock_guard<std::mutex> guard(mutexSprite);
        if (width > 0 && height > 0 && IsImageValid(image))
        {
            if (bAspectRatio)
            {
                UtilsFunctionLibrary::ImageResize(image, width, height);
            }
            else
            {
                ImageResize(&image, width, height);
            }
            if (bTextureLoaded)
            {
                UpdateTexture(texture, image.data);
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
        if (bTextureLoaded && IsTextureValid(texture))
        {
            ::UnloadTexture(texture);
            TraceLog(LOG_DEBUG, "Unloaded Texture [ID %d] from - %s", texture.id, filePath.c_str());
            texture = {};
            bTextureLoaded = false;
        }
    }

    void Sprite::UnloadImage()
    {
        if (bImageLoaded && IsImageValid(image))
        {
            ::UnloadImage(image);
            TraceLog(LOG_DEBUG, "Unloaded Image from - %s", filePath.c_str());
            image = {};
            bImageLoaded = false;
        }
    }

}  // namespace ClassicLauncher
