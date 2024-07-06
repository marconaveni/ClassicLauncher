#include "Render.h"

#include "Math.h"
#include "raymath.h" // Required for: Vector2Clamp()
#include "raylib.h"

Render::Render() : renderTexture(),
width(0),
height(0),
newWidth(0),
newHeight(0),
scale(1),
bMaintainAspectRatio(true),
virtualMouse(Vector2{ 0, 0 })
{
}

void Render::RenderValues()
{
	const Vector2 mouse = GetMousePosition();
	newWidth = GetWidthRender();
	newHeight = GetHeightRender();


	if (bMaintainAspectRatio)
	{
		scale = Math::Min<float>(static_cast<float>(GetScreenWidth()) / static_cast<float>(newWidth), static_cast<float>(GetScreenHeight()) / static_cast<float>(newHeight));
		virtualMouse.x = (mouse.x - (static_cast<float>(GetScreenWidth()) - (static_cast<float>(newWidth) * scale)) * 0.5f) / scale;
		virtualMouse.y = (mouse.y - (static_cast<float>(GetScreenHeight()) - (static_cast<float>(newHeight) * scale)) * 0.5f) / scale;
		virtualMouse = Vector2Clamp(virtualMouse, Vector2{ 0, 0 }, Vector2{ static_cast<float>(newWidth), static_cast<float>(newHeight) });
	}
	else
	{
		scale = 1;
		virtualMouse.x = (mouse.x / static_cast<float>(GetScreenWidth())) * static_cast<float>(width);
		virtualMouse.y = (mouse.y / static_cast<float>(GetScreenHeight())) * static_cast<float>(height);
	}
}

Render* Render::GetInstance()
{
	static Render object;
	return &object;
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

void Render::EndRender()
{
	EndTextureMode();
}

void Render::DrawRender() const
{
	// Draw render texture to screen, properly scaled
	DrawTexturePro(renderTexture.texture, Rectangle{ 0.0f, 0.0f, static_cast<float>(renderTexture.texture.width), static_cast<float>(-renderTexture.texture.height) },
		Rectangle{
					   (static_cast<float>(GetScreenWidth()) - (static_cast<float>(newWidth) * scale)) * 0.5f, (static_cast<float>(GetScreenHeight()) - (static_cast<float>(newHeight) * scale)) * 0.5f,
					   static_cast<float>(newWidth) * scale, static_cast<float>(newHeight) * scale
		}, Vector2{ 0, 0 }, 0.0f, WHITE);
}


Vector2 Render::GetRenderScale() const
{
	const float scaleWidth = static_cast<float>(GetScreenWidth()) / static_cast<float>(width);
	const float scaleHeight = static_cast<float>(GetScreenHeight()) / static_cast<float>(height);
	return Vector2{ scaleWidth, scaleHeight };
}

Vector2 Render::GetMousePositionRender() const
{
	return virtualMouse;
}

int Render::GetWidthRender() const
{
	return (bMaintainAspectRatio) ? width : GetScreenWidth();
}

int Render::GetHeightRender() const
{
	return (bMaintainAspectRatio) ? height : GetScreenHeight();
}

int Render::GetScreenWidthGame() const
{
	return width;
}

int Render::GetScreenHeightGame() const
{
	return height;
}
