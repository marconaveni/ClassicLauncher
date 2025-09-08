#ifndef GUI_VIDEO_PLAYER_H
#define GUI_VIDEO_PLAYER_H

#include <memory>
#include <string>
#include "Entity/EntityGui.h"
#include "Video/VideoPlayer.h"

namespace ClassicLauncher
{
    class VideoPlayer;
    class GuiComponent;
    class Application;
    class EntityGui;

    class GuiVideoPlayer : public EntityGui
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
        void End() override;
        bool IsPlaying();
        bool IsPlayingFullscreen();

    private:

        std::unique_ptr<VideoPlayer> mPlayer;
        std::unique_ptr<VideoPlayer> mPlayerFullScreen;
        std::string mFilePath;
    };

}  // namespace ClassicLauncher

#endif  // GUI_VIDEO_PLAYER_H