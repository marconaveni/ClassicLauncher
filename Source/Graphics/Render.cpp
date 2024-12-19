#include "Render.h"

#include "Utils/Math.h"
#include "raylib.h"
#include "raymath.h"  // Required for: Vector2Clamp()

namespace ClassicLauncher
{

    Render::Render()
        : renderTexture(), width(0), height(0), newWidth(0), newHeight(0), scale(1), bMaintainAspectRatio(true), virtualMouse(Vector2{ 0 })
    {
    }

    void Render::RenderValues()
    {
        const Vector2 mouse = GetMousePosition();
        const float screenWidth = static_cast<float>(GetScreenWidth());
        const float screenHeight = static_cast<float>(GetScreenHeight());
        newWidth = static_cast<float>(GetWidthRender());
        newHeight = static_cast<float>(GetHeightRender());

        if (bMaintainAspectRatio)
        {
            scale = Math::Min<float>(screenWidth / newWidth, screenHeight / newHeight);
            virtualMouse.x = (mouse.x - (screenWidth - (newWidth * scale)) * 0.5f) / scale;
            virtualMouse.y = (mouse.y - (screenHeight - (newHeight * scale)) * 0.5f) / scale;
            virtualMouse = Vector2Clamp(virtualMouse, Vector2{ 0 }, Vector2{ newWidth, newHeight });
        }
        else
        {
            scale = 1;
            virtualMouse.x = (mouse.x / screenWidth) * width;
            virtualMouse.y = (mouse.y / screenHeight) * height;
        }
    }

    void Render::LoadRender(const int screenWidth, const int screenHeight)
    {
        width = screenWidth;
        height = screenHeight;
        renderTexture = LoadRenderTexture(screenWidth, screenHeight);
        GenTextureMipmaps(&renderTexture.texture);
        SetTextureFilter(renderTexture.texture, TEXTURE_FILTER_TRILINEAR);
        GenTextureMipmaps(&renderTexture.depth);
        SetTextureFilter(renderTexture.depth, TEXTURE_FILTER_TRILINEAR);
    }

    void Render::BeginRender()
    {
        RenderValues();
        BeginTextureMode(renderTexture);
        ClearBackground(LIGHTGRAY);
    }

    void Render::EndRender() { EndTextureMode(); }

    void Render::DrawRender() const
    {
        const float screenWidth = static_cast<float>(GetScreenWidth());
        const float screenHeight = static_cast<float>(GetScreenHeight());
        const float textureWidth = static_cast<float>(renderTexture.texture.width);
        const float textureHeight = static_cast<float>(renderTexture.texture.height);

        const Texture2D texture = renderTexture.texture;
        const Rectangle source = Rectangle{ 0.0f, 0.0f, textureWidth, -textureHeight };
        const Rectangle dest = Rectangle{
            (screenWidth - (newWidth * scale)) * 0.5f, (screenHeight - (newHeight * scale)) * 0.5f, newWidth * scale, newHeight * scale
        };

        // Draw render texture to screen, properly scaled
        DrawTexturePro(texture, source, dest, Vector2{ 0 }, 0.0f, WHITE);
    }

    void Render::Unload()
    {
        if (IsTextureValid(renderTexture.texture))
        {
            UnloadRenderTexture(renderTexture);
        }
    }

    Vector2 Render::GetRenderScale() const
    {
        const float scaleWidth = static_cast<float>(GetScreenWidth()) / width;
        const float scaleHeight = static_cast<float>(GetScreenHeight()) / height;
        return Vector2{ scaleWidth, scaleHeight };
    }

    Vector2 Render::GetMousePositionRender() const { return virtualMouse; }

    int Render::GetWidthRender() const { return (bMaintainAspectRatio) ? static_cast<int>(width) : GetScreenWidth(); }

    int Render::GetHeightRender() const { return (bMaintainAspectRatio) ? static_cast<int>(height) : GetScreenHeight(); }

}  // namespace ClassicLauncher