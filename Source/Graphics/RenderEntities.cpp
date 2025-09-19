#include "RenderEntities.h"
#include <format>
#include "Helper.h"
#include "rl_wrap.h"
#include "Utils/Math.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Themes/ThemesManager.h"
#include "Graphics/SpriteManager.h"
#include "Window/RayWindow.h"

namespace ClassicLauncher
{

#ifdef _DEBUG
    static bool bEnable = false;
#endif

    RenderEntities::RenderEntities(SpriteManager* spriteManagerReference)
        : m_spriteManagerReference(spriteManagerReference)
    {
    }

    void RenderEntities::DrawEntities(const std::vector<std::unique_ptr<Entity>>& entities)
    {
#ifdef _DEBUG
        if (Keyboard::IsReleased(Keyboard::Key::FIVE))
        {
            bEnable = !bEnable;
        }
#endif
        for (const auto& entity : entities)
        {
            DrawEntity(entity.get());
        }
    }

    void RenderEntities::DrawEntity(Entity* entity)
    {
        const Texture* texture = m_spriteManagerReference->GetTexture(entity->mTextureName);

        // if (texture && entity->mToDraw && entity->mTextureName != "transparent")  // todo verify render
        if (texture && entity->mToDraw)
        {
            entity->mTransform.SetTransforms(ThemesManager::GetScaleTexture());

            if (entity->mScissorMode)
            {
                RectFloat scissorArea = entity->mScissorArea;
                scissorArea.width = scissorArea.width * entity->mTransform.root.scale.x * ThemesManager::GetScaleTexture();
                scissorArea.height = scissorArea.height * entity->mTransform.root.scale.y * ThemesManager::GetScaleTexture();
                rlw::BeginScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
            }

            rlw::DrawTexturePro(*texture,
                                entity->mTransform.GetSource(),
                                entity->mTransform.GetTransform(),
                                Vector2f{ 0, 0 },
                                entity->mTransform.rotation,
                                entity->mTransform.color);
            entity->Draw();
#ifdef _DEBUG
            DrawDebug(entity);
#endif  // _DEBUG

            if (entity->mScissorMode)
            {
                rlw::EndScissorMode();
                entity->DisableScissorMode();
            }
            entity->mToDraw = false;
        }
    }

    void RenderEntities::DrawDebug(Entity* entity)
    {


        const RectFloat& rectDrawArea = entity->mTransform.GetTransform();  //{ x, y, scale.x, scale.y };
        const Vector2f vec = {};  // TODO Refactor  Application::Get().GetRenderScreen()->GetMousePositionRender();
        RectFloat point = RectFloat{ rectDrawArea.x, rectDrawArea.y, rectDrawArea.width, rectDrawArea.height };
        if (Math::CheckCollisionPointRec(RayWindow::GetVirtualMouse(), point) && bEnable)
        {
            rlw::DrawRectangleLinesEx(rectDrawArea, 2, Color::Red);
            if (Mouse::IsPressed(Mouse::LEFT))
            {
                PRINT(std::format("nameID: {}", entity->mNameId).c_str(), 5.0f);
            }
        }
        else if (bEnable)
        {
            rlw::DrawRectangleLinesEx(rectDrawArea, 1, Color::Cyan);
        }
        if (entity->mScissorMode && bEnable)
        {
            const Color tint = Color(255, 0, 0, 55);
            RectFloat scissorArea = entity->mScissorArea;
            scissorArea.width = scissorArea.width * entity->mTransform.root.scale.x * ThemesManager::GetScaleTexture();
            scissorArea.height = scissorArea.height * entity->mTransform.root.scale.y * ThemesManager::GetScaleTexture();
            rlw::DrawRectangle(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height, tint);
        }

    }


}  // namespace ClassicLauncher
