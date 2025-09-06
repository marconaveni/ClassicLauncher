#ifndef SPRITE_H
#define SPRITE_H

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

#include "rl_wrap.h"

namespace ClassicLauncher
{

    class Sprite
    {
    public:

        Sprite();
        Sprite(const Sprite&) = delete;
        Sprite& operator=(const Sprite&) = delete;
        ~Sprite();
        void Load(const std::string& file, int width = 0, int height = 0, bool bAspectRatio = true);
        void Load(const rlw::Image& newImage, int width = 0, int height = 0, bool bAspectRatio = true);
        void Stop();
        void Join();
        rlw::Texture2D* GetTexture();
        rlw::Image* GetImage();
        void ResizeImage(int width, int height, bool bAspectRatio);
        void Unload();
        void UnloadTexture();
        void UnloadImage();

    private:

        std::mutex mMutexSprite;
        std::atomic<bool> mIsKeepRunning;
        std::atomic<bool> mIsImageLoaded;
        std::atomic<bool> mIsTextureLoaded;
        std::thread mWorkerThread;
        rlw::Image mImage;
        rlw::Texture2D mTexture;
        std::string mFilePath;
        void LoadImage(int width, int height, bool bAspectRatio);

    };

}  // namespace ClassicLauncher

#endif