#ifndef SPRITE_H
#define SPRITE_H

#include <atomic>
#include <string>
#include <thread>
#include <mutex>
#include "raylib.h"

namespace ClassicLauncher
{

    class Sprite
    {
    
    public:

        Sprite();
        Sprite(const Sprite&) = delete;
        Sprite& operator = (const Sprite&) = delete;
        ~Sprite();
        void Load(const std::string file, const int width = 0, const int height = 0, bool bAspectRatio = true);
        void Stop();
        void Join();

    private:

        std::mutex mutexSprite;
        std::atomic<bool> bKeepRunning;
        std::atomic<bool> bImageLoaded;
        std::atomic<bool> bTextureLoaded;
        std::thread workerThread;
        Image image;
        Texture2D texture;
        std::string filePath;
        void LoadImage(const int width, const int height, bool bAspectRatio);

    public:

        Texture2D* GetSprite();
        void ResizeImage(const int width, const int height, bool bAspectRatio);
        void Unload();
        void UnloadTexture();
        void UnloadImage();
    };


} // ClassicLauncher


#endif