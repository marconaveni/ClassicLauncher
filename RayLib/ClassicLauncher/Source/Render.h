#pragma once



#include "raylib.h"

struct RenderTexture;
struct Vector2;

class Render
{
    RenderTexture renderTexture;
    int width;
    int height;
    int newWidth;
    int newHeight;
    float scale;
    bool bMaintainAspectRatio;
    Vector2 virtualMouse;

    Render();

    void RenderValues();

public:
    static Render* GetInstance();

    void LoadRender(int screenWidth, int screenHeight);

    void BeginRender();

    void EndRender();

    void DrawRender() const;

    Vector2 GetRenderScale() const;

    Vector2 GetMousePositionRender() const;

private:
    int GetWidthRender() const;

    int GetHeightRender() const;

public:
    int GetScreenWidthGame() const;

    int GetScreenHeightGame() const;
};
