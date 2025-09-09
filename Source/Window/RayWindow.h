#ifndef RAY_WINDOW_H
#define RAY_WINDOW_H


#include "Window/Window.h"

namespace ClassicLauncher
{

    class RayWindow : public Window
    {
    public:
        RayWindow() = default;
        ~RayWindow();
        virtual void Init(int width, int height, const std::string& title) override;
        virtual bool ShouldClose() override;
        virtual void Close() override;

        virtual void SetState(unsigned int flags) override;
        virtual void ClearState(unsigned int flags) override;
        virtual bool IsState(unsigned int flag) override;
        virtual void SetSize(int width, int height) override;
        virtual void SetPosition(int x, int y) override;
        virtual void SetIcons(const std::vector<std::string>& pathIcons) override;
        virtual void SetExitKey(int key) override;
        virtual void SetTargetFPS(int fps) override;
        virtual int GetFPS() override;
        virtual float GetFrameTime() override;

        virtual int GetScreenWidth() override;
        virtual int GetScreenHeight() override;

        virtual int GetCurrentMonitor() override;
        virtual int GetMonitorWidth(int monitor) override;
        virtual int GetMonitorHeight(int monitor) override;
        virtual Vector2i GetMonitorPosition(int monitor) override;
        bool ToggleFullscreen();

        [[nodiscard]] bool IsResize() const { return m_isResized; }

    private:

        std::string m_title{};
        Vector2i m_size{};
        Vector2i m_position{};
        bool m_isResized{};
        bool m_isFullScreen{};
    };

};  // namespace ClassicLauncher

#endif