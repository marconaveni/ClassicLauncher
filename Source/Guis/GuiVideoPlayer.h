#ifndef GUI_VIDEO_PLAYER_H
#define GUI_VIDEO_PLAYER_H

#include <memory>
#include <string>

#include "Entity/Entity.h"
#include "Video/VideoPlayer.h"
#include "Animations/Animatable.h"

namespace ClassicLauncher
{
    class VideoPlayer;
    class GuiBase;
    class Application;
    class RenderTexture;

    class GuiVideoPlayer : public Entity, public Animatable
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
        
        RenderTexture* m_renderTexture = nullptr;
        std::unique_ptr<VideoPlayer> m_player;
        std::unique_ptr<VideoPlayer> m_playerFullScreen;
        std::string m_filePath;
        float m_renderScale = 1;
        void VideoFadeinAnimate(float time);
    };

} // namespace ClassicLauncher

#endif // GUI_VIDEO_PLAYER_H