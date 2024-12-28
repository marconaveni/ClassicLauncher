#ifndef GUI_VIDEO_PLAYER_H
#define GUI_VIDEO_PLAYER_H

#include <memory>
#include <string>
#include <vector>

#include "Application.h"
#include "GuiComponent.h"
#include "Video/VideoPlayer.h"

namespace ClassicLauncher
{
    class VideoPlayer;
    class GuiComponent;
    class Application;

    class GuiVideoPlayer : public GuiComponent
    {
    private:

        Application* mApplication;
        std::unique_ptr<VideoPlayer> mPlayer;

    public:

        GuiVideoPlayer();
        virtual ~GuiVideoPlayer() override = default;
        EntityType GetType() const override { return EntityType::GuiVideoPlayer; }
        bool Init(std::string path, int width, int height);
        void Stop();
        void Update() override;
        void Draw() override;
        void End() override;

    private:

    };

}  // namespace ClassicLauncher

#endif  // GUI_VIDEO_PLAYER_H