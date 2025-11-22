#ifndef SPRITE_H
#define SPRITE_H

#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <memory>
#include <filesystem>

#include "Graphics/Image.h"
#include "Graphics/Texture.h"

namespace ClassicLauncher
{

    class Sprite
    {
    public:

        Sprite() = default;
        Sprite(const Sprite&) = delete;
        Sprite& operator=(const Sprite&) = delete;
        ~Sprite();
        void Load(const std::filesystem::path& file, int width = 0, int height = 0, bool aspectRatio = true);
        void Load(Image& newImage, int width = 0, int height = 0, bool aspectRatio = true);
        void Stop();
        void Join();
        Texture* GetTexture();
        Image* GetImage();
        void ResizeImage(int width, int height, bool aspectRatio);
        void Unload();
        void UnloadTexture();
        void UnloadImage();

    private:

        std::mutex m_mutexSprite{};
        std::atomic<bool> m_isKeepRunning{false};
        std::atomic<bool> m_isImageLoaded{false};
        std::atomic<bool> m_isTextureLoaded{false};
        std::thread m_workerThread{};
        Image m_image{};
        std::unique_ptr<Texture> m_texture{};
        std::string m_filePath{};
        void LoadImage(const std::filesystem::path& file, int width, int height, bool aspectRatio);
    };

} // namespace ClassicLauncher

#endif