#include "Sprite.h"

#include <atomic>
#include <string>
#include <thread>

#include "ClassicAssert.h"
#include "Utils/Log.h"
#include "Utils/Utils.h"

namespace ClassicLauncher
{

    // Sprite::Sprite()
    //     : m_isKeepRunning(false), m_isImageLoaded(false), m_isTextureLoaded(false), m_image(), m_texture()
    // {
    // }

    Sprite::~Sprite()
    {
        Stop();
        Join();
        Unload();
        LOG(LOG_CLASSIC_TRACE, "Sprite - thread stopped and class destroyed");
    }

    void Sprite::Load(const std::filesystem::path& file, const int width, const int height, bool bAspectRatio)
    {
        if (!m_isKeepRunning && !m_isTextureLoaded && !m_isImageLoaded)
        {
            Join();
            m_isKeepRunning = true;
            //m_filePath = file;
            LOG(LOG_CLASSIC_TRACE, "Sprite - starting thread");
            m_workerThread = std::thread(&Sprite::LoadImage, this, file, width, height, bAspectRatio);
        }
    }

    void Sprite::Load(Image& newImage, const int width, const int height, const bool bAspectRatio)
    {
        if (newImage.IsValid())
        {
            Unload();
            newImage.CopyTo(m_image);
            m_filePath = "[loaded from memory]";
            ResizeImage(width, height, bAspectRatio);
            m_isImageLoaded = m_image.IsValid();
            LOG(LOG_CLASSIC_TRACE, "Image copied successfully");
        }
    }

    void Sprite::Stop()
    {
        m_isKeepRunning = false; // Sinaliza para encerrar
    }

    void Sprite::Join()
    {
        if (m_workerThread.joinable())
        {
            m_workerThread.join();
        }
    }

    void Sprite::LoadImage(const std::filesystem::path& file,const int width, const int height, bool bAspectRatio)
    {
        // std::this_thread::sleep_for(std::chrono::seconds(1)); //for test
        if (m_isKeepRunning)
        {
            // std::this_thread::sleep_for(std::chrono::seconds(1)); //for test
            m_image.LoadFromFile(file);
            if (m_image.IsValid())
            {
                ResizeImage(width, height, bAspectRatio);
                m_isImageLoaded = m_image.IsValid();
                LOG(LOG_CLASSIC_TRACE, "Image loaded successfully from - \"%s\"", m_filePath.c_str());
            }
            else
            {
                LOG(LOG_CLASSIC_WARNING, "Failed to load Image - \"%s\"", m_filePath.c_str());
            }
        }
        LOG(LOG_CLASSIC_TRACE, "LoadImage - finished");
        Stop();
    }

    Texture* Sprite::GetTexture()
    {
        if (!m_isTextureLoaded && m_isImageLoaded)
        {
            // mTexture = rlw::LoadTextureFromImage(mImage);
            m_texture = std::make_unique<Texture>();
            m_texture->LoadFromImage(&m_image);
            m_isTextureLoaded = m_texture->IsValid();
            LOG(LOG_CLASSIC_TRACE, "Texture loaded [ID %d] from Image - \"%s\"", m_texture->GetId(), m_filePath.c_str());
            UnloadImage();
        }
        if (m_isTextureLoaded)
        {
            return m_texture.get();
        }
        return nullptr;
    }

    Image* Sprite::GetImage()
    {
        if (m_isImageLoaded)
        {
            return &m_image;
        }
        return nullptr;
    }

    void Sprite::ResizeImage(const int width, const int height, bool bAspectRatio)
    {
        std::lock_guard<std::mutex> guard(m_mutexSprite);
        if (width > 0 && height > 0 && m_image.IsValid())
        {
            if (bAspectRatio)
            {
                Utils::ImageResize(m_image, width, height);
            }
            else
            {
                //rlw::ImageResize(&mImage, width, height);
                m_image.Resize(width, height);
            }
            if (m_isTextureLoaded)
            {
                m_texture->Update(m_image.data);
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
        if (m_isTextureLoaded && m_texture->IsValid())
        {
            m_texture->Unload();
            LOG(LOG_CLASSIC_TRACE, "Unloaded Texture [ID %d] from - \"%s\"", m_texture->GetId(), m_filePath.c_str());
            m_isTextureLoaded = false;
        }
    }

    void Sprite::UnloadImage()
    {
        if (m_isImageLoaded && m_image.IsValid())
        {
            m_image.Unload();
            LOG(LOG_CLASSIC_TRACE, "Unloaded Image from - %s", m_filePath.c_str());
            m_image = {};
            m_isImageLoaded = false;
        }
    }

} // namespace ClassicLauncher
