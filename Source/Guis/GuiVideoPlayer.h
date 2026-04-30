#ifndef GUI_VIDEO_PLAYER_H
#define GUI_VIDEO_PLAYER_H

#include <filesystem>
#include <memory>
#include <string>

#include "Animations/Animatable.h"
#include "Guis/GuiBase.h"
#include "Video/VideoPlayer.h"


namespace ClassicLauncher
{

    class RenderTexture;
    class Window;

    class GuiVideoPlayer : public Entity, public Animatable
    {
    public:

        explicit GuiVideoPlayer(const EntityContext& entityContext, Window* window);
        EntityType GetType() const override { return EntityType::GuiVideoPlayerClass; }
        void Init(const std::filesystem::path& path, int width, int height);
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

        RenderTexture* m_renderTexture{nullptr};
        GuiBase m_gui;
        std::unique_ptr<VideoPlayer> m_player{nullptr};
        std::unique_ptr<VideoPlayer> m_playerFullScreen{nullptr};
        std::filesystem::path m_filePath{};
        float m_renderScale{1};
        void VideoFadeinAnimate(float time, Entity* entity);
    };

} // namespace ClassicLauncher

#endif // GUI_VIDEO_PLAYER_H