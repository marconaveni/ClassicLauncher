#ifndef GUI_VIDEO_PLAYER_H
#define GUI_VIDEO_PLAYER_H

#include <memory>
#include <string>

#include "Entity/Entity.h"
#include "Video/VideoPlayer.h"

namespace ClassicLauncher
{
    class VideoPlayer;
    class GuiBase;
    class Application;
    class RenderTexture;

    class GuiVideoPlayer : public Entity
    {
    public:

        GuiVideoPlayer();
        EntityType GetType() const override { return EntityType::GuiVideoPlayerClass; }
        bool Init(const std::string& path, int width, int height);
        void InitFullscreen();
        void Stop();
        void StopFullscreen();
        void Update() override;
        void Draw() override;
        void DrawVideo();
        void End() override;
        bool IsPlaying();
        bool IsPlayingFullscreen();
        
    private:
        
        RenderTexture* m_renderTexture;
        std::unique_ptr<VideoPlayer> mPlayer;
        std::unique_ptr<VideoPlayer> mPlayerFullScreen;
        std::string mFilePath;
        float m_renderScale = 1;
    };

} // namespace ClassicLauncher

#endif // GUI_VIDEO_PLAYER_H