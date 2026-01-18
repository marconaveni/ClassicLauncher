#ifndef RAY_WINDOW_H
#define RAY_WINDOW_H

#include "Utils/ConfigurationManager.h"
#include "Window/Window.h"

namespace ClassicLauncher
{

    class RayWindow : public Window
    {
    public:

        RayWindow() = default;
        ~RayWindow();
        void Init(int width, int height, const std::string& title, ConfigurationManager& configManager);
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
        static void ShowCursor(bool isEnable);
        static void MinimizeWindow();
        static void RestoreWindow();
        static void SetWindowFocused();

        void PoolEvents();
        static void SetConfigFlags(unsigned int flags);

        [[nodiscard]] static bool IsResize();

        struct Flags
        {
            inline static constexpr unsigned int Vsync = 0x00000040;      // Set to try enabling V-Sync on GPU
            inline static constexpr unsigned int Fullscreen = 0x00000002; // Set to run program in fullscreen
            inline static constexpr unsigned int Resizable = 0x00000004;  // Set to allow resizable window
            inline static constexpr unsigned int Undecorated = 0x00000008; // Set to disable window decoration (frame and buttons)
            inline static constexpr unsigned int Hidden = 0x00000080;    // Set to hide window
            inline static constexpr unsigned int Minimized = 0x00000200; // Set to minimize window (iconify)
            inline static constexpr unsigned int Maximized = 0x00000400; // Set to maximize window (expanded to monitor)
            inline static constexpr unsigned int Unfocused = 0x00000800; // Set to window non focused
            inline static constexpr unsigned int Topmost = 0x00001000;   // Set to window always on top
            inline static constexpr unsigned int AlwaysRun = 0x00000100; // Set to allow windows running while minimized
            inline static constexpr unsigned int Transparent = 0x00000010; // Set to allow transparent framebuffer
            inline static constexpr unsigned int HighDPI = 0x00002000;     // Set to support HighDPI
            inline static constexpr unsigned int MousePassthrough = 0x00004000; // Set to support mouse passthrough, only supported when FLAG WINDOW UNDECORATED
            inline static constexpr unsigned int BorderlessWindowedMode = 0x00008000; // Set to run program in borderless windowed mode
            inline static constexpr unsigned int Msaa4X = 0x00000020; // Set to try enabling MSAA 4X
            inline static constexpr unsigned int Interlaced = 0x00010000; // Set to try enabling interlaced video format (for V3D)
        };

    private:

        friend class RenderScreen;

        static Vector2f m_virtualMouse;
        std::string m_title{};
        Vector2i m_size{};
        Vector2i m_position{};
        bool m_isFullScreen{false};
        bool m_isReady{false};
        ConfigurationManager* m_configManager{nullptr};

        bool isFullScreen() const { return m_isFullScreen; };
        bool ToggleFullscreen();
        void Unload();
    };

}; // namespace ClassicLauncher

#endif