#ifndef SPRITE_H
#define SPRITE_H

#include <atomic>
#include <string>
#include <thread>
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
        void Load(const std::string file);
        void Stop();

    private:

        std::atomic<bool> bKeepRunning;
        std::atomic<bool> bImageLoaded;
        std::atomic<bool> bTextureLoaded;
        std::thread workerThread;
        Image image;
        Texture2D texture;
        std::string filePath;
        void LoadImage();

    public:

        Texture2D* GetSprite();
        void Unload();
        void UnloadTexture();
        void UnloadImage();
    };


} // ClassicLauncher


#endif