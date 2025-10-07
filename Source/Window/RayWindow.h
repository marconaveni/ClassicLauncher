#ifndef RAY_WINDOW_H
#define RAY_WINDOW_H

#include "Utils/ConfigurationManager.h"
#include "Window/Window.h"

namespace ClassicLauncher
{

    class RayWindow : public Window
    {
    public:

        RayWindow(ConfigurationManager& configManager);
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

        static int GetFPS();
        static float GetFrameTime();
        static int GetScreenWidth();
        static int GetScreenHeight();
        static int GetCurrentMonitor();
        static int GetMonitorWidth(int monitor);
        static int GetMonitorHeight(int monitor);
        static Vector2i GetMonitorPosition(int monitor);
        static Vector2i GetMousePosition();
        static Vector2f GetVirtualMouse();
        static void ShowCursor(bool bEnable);     

        void PoolEvents();
        void SetConfigFlags(unsigned int flags);

        [[nodiscard]] static bool IsResize();

        struct Flags
        {
            inline static constexpr unsigned int Vsync = 0x00000040;
            inline static constexpr unsigned int Resizable = 0x00000004;
            inline static constexpr unsigned int Undecorated = 0x00000008;
        };

    private:

        friend class RenderScreen;

        static Vector2f m_virtualMouse;
        std::string m_title{};
        Vector2i m_size{};
        Vector2i m_position{};
        bool m_isFullScreen{};
        bool m_isReady{};
        ConfigurationManager* m_configManager{};

        bool ToggleFullscreen();
        void Unload();
    };

}; // namespace ClassicLauncher

#endif