#include "RenderEntities.h"

#include <format>

#include "Graphics/SpriteManager.h"
#include "Helper.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Themes/ThemesManager.h"
#include "Utils/Math.h"
#include "Window/RayWindow.h"
#include "raylib.h" // isso não pode ficar aqui
#include "rl_wrap.h"

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

        m_renderScale = ThemesManager::GetScaleRenderer();

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

            // A lógica de cálculo da transformação final agora é diferente

            // Use a mWorldTransform para obter os valores finais

            

            RectFloat sourceRect{
                entity->m_transform.source.x * m_renderScale,
                entity->m_transform.source.y * m_renderScale,
                entity->m_transform.source.width * m_renderScale ,
                entity->m_transform.source.height * m_renderScale
            };

            //float finalScale = 2.0f;

            RectFloat finalTransformRect{
                entity->m_worldTransform.position.x * entity->m_worldTransform.scale.x * m_renderScale,
                entity->m_worldTransform.position.y * entity->m_worldTransform.scale.y * m_renderScale,
                entity->m_transform.position.width * entity->m_worldTransform.scale.x  * m_renderScale, // Largura base * escala final
                entity->m_transform.position.height * entity->m_worldTransform.scale.y * m_renderScale   // Altura base * escala final
            };

            // ... Lógica do Scissor Mode ...

            rlw::DrawTexturePro(*texture,
                                sourceRect,         /* RectFloat{0, 562, 21, 720}, posição spritesheet */
                                finalTransformRect, /* RectFloat{0, 0, 1280, 720} posx posy tam_rect  larg_rect */
                                Vector2f{0, 0},
                                entity->m_worldTransform.rotation,
                                entity->m_worldTransform.color);

            rlw::DrawRectangleLinesEx(finalTransformRect, 1, Color::Cyan);

           // DrawText(entity->mNameId.c_str(), finalTransformRect.x, finalTransformRect.y, 16, ::WHITE);

            //rlw::DrawTexturePro(*texture,
            //    RectFloat{ 0 , 562 , 21 ,720 }, //    posição spritesheet
            //    RectFloat{ 0 , 0 , 1280 ,720  }, //   posx posy tam_rect  larg_rect
            //    Vector2f{0, 0},
            //    entity->m_worldTransform.rotation,
            //    entity->m_worldTransform.color);

            // if (entity->mScissorMode)
            // {
            //     RectFloat scissorArea = entity->mScissorArea;
            //     scissorArea.width =
            //         scissorArea.width * entity->m_transform.root.scale.x * ThemesManager::GetScaleTexture();
            //     scissorArea.height =
            //         scissorArea.height * entity->m_transform.root.scale.y * ThemesManager::GetScaleTexture();
            //     rlw::BeginScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
            // }

            entity->Draw();

            // if (entity->mScissorMode)
            // {
            //     rlw::EndScissorMode();
            //     entity->DisableScissorMode();
            // }
            entity->mToDraw = false;
        }
    }

    void RenderEntities::DrawDebug(Entity* entity)
    {


        // const RectFloat& rectDrawArea = entity->mTransform.GetTransform(); //{ x, y, scale.x, scale.y };
        // const Vector2f vec = {}; // TODO Refactor  Application::Get().GetRenderScreen()->GetMousePositionRender();
        // RectFloat point = RectFloat{rectDrawArea.x, rectDrawArea.y, rectDrawArea.width, rectDrawArea.height};
        // if (Math::CheckCollisionPointRec(RayWindow::GetVirtualMouse(), point) && bEnable)
        // {
        //     rlw::DrawRectangleLinesEx(rectDrawArea, 2, Color::Red);
        //     if (Mouse::IsPressed(Mouse::LEFT))
        //     {
        //         PRINT(std::format("nameID: {}", entity->mNameId).c_str(), 5.0f);
        //     }
        // }
        // else if (bEnable)
        // {
        //     rlw::DrawRectangleLinesEx(rectDrawArea, 1, Color::Cyan);
        // }
        // if (entity->mScissorMode && bEnable)
        // {
        //     const Color tint = Color(255, 0, 0, 55);
        //     RectFloat scissorArea = entity->mScissorArea;
        //     scissorArea.width =
        //         scissorArea.width * entity->m_transform.root.scale.x * ThemesManager::GetScaleTexture();
        //     scissorArea.height =
        //         scissorArea.height * entity->m_transform.root.scale.y * ThemesManager::GetScaleTexture();
        //     rlw::DrawRectangle(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height, tint);
        // }
    }


} // namespace ClassicLauncher
