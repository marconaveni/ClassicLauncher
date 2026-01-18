#ifndef RENDER_SCREEN_H
#define RENDER_SCREEN_H

#include <memory>

#include "Data/Rectangle.h"
#include "Data/Vector2.h"

namespace ClassicLauncher
{
    class RenderTexture;

    class RenderScreen
    {
    public:

        RenderScreen() = default;
        ~RenderScreen();
        void Init(int screenWidth, int screenHeight);
        void Clear();
        void BeginRender();
        void EndRender();
        void Draw();
        void Unload();
        [[nodiscard]] Vector2f GetRenderScale() const;
        [[nodiscard]] Vector2f GetMousePositionRender() const;
        [[nodiscard]] int GetScreenWidthGame() const { return static_cast<int>(m_width); }
        [[nodiscard]] int GetScreenHeightGame() const { return static_cast<int>(m_height); }

    private:

        void UpdateValues();
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;

        std::unique_ptr<RenderTexture> m_renderTexture{};
        RectFloat m_source{};
        RectFloat m_dest{};
        float m_width{0};
        float m_height{0};
        float m_newWidth{0};
        float m_newHeight{0};
        float m_scale{1};
        bool m_isMaintainAspectRatio{true};
        Vector2f m_virtualMouse{};
    };

} // namespace ClassicLauncher

#endif