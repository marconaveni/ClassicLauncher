#include "RenderEntities.h"

#include <format>

#include "Components/FocusComponent.h"
#include "Graphics/SpriteManager.h"
#include "Helper.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Themes/ThemesManager.h"
#include "Utils/Math.h"
#include "Window/RayWindow.h"
#include "raylib.h" // isso não pode ficar aqui
#include "rl_wrap.h"
#include <iostream>

namespace ClassicLauncher
{

#ifdef _DEBUG
    static bool enableDebug = false;
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
            enableDebug = !enableDebug;
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



        // if (entity->mScissorMode)
        // {
        //     RectFloat scissorArea = entity->mScissorArea;
        //     scissorArea.width =
        //         scissorArea.width * entity->m_transform.root.scale.x * ThemesManager::GetScaleTexture();
        //     scissorArea.height =
        //         scissorArea.height * entity->m_transform.root.scale.y * ThemesManager::GetScaleTexture();
        //     rlw::BeginScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
        // }

        // if (texture && entity->mToDraw && entity->mTextureName != "transparent")  // todo verify render
        if (texture && entity->mToDraw)
        {

            RectFloat sourceRect{entity->m_transform.source.x * m_renderScale,
                                 entity->m_transform.source.y * m_renderScale,
                                 entity->m_transform.source.width * m_renderScale,
                                 entity->m_transform.source.height * m_renderScale};
    

            entity->m_finalTransformRect = RectFloat{
                (entity->m_worldTransform.position.x + (entity->m_worldTransform.offset.x * entity->m_worldTransform.scale.x)) * m_renderScale,
                (entity->m_worldTransform.position.y + (entity->m_worldTransform.offset.y * entity->m_worldTransform.scale.y)) * m_renderScale,
                entity->m_transform.position.width * entity->m_worldTransform.scale.x * m_renderScale, // Largura base * escala final
                entity->m_transform.position.height * entity->m_worldTransform.scale.y * m_renderScale // Altura base * escala final
            };
            
            
            Vector2f finalOrigin = {
                entity->m_transform.origin.x * entity->m_worldTransform.scale.x * m_renderScale,
                entity->m_transform.origin.y * entity->m_worldTransform.scale.y * m_renderScale
            };

            rlw::DrawTexturePro(*texture,
                                sourceRect,         /* RectFloat{0, 562, 21, 720}, posição spritesheet */
                                entity->m_finalTransformRect, /* RectFloat{0, 0, 1280, 720} posx posy tam_rect  larg_rect */
                                finalOrigin,
                                entity->m_worldTransform.rotation,
                                entity->m_worldTransform.color);
            
        }

        entity->Draw();     
        
        // if (entity->mScissorMode)
        // {
        //     rlw::EndScissorMode();
        //     entity->DisableScissorMode();
        // }

#ifdef _DEBUG
        if (enableDebug)
        {
            DrawDebug(entity);
        }
#endif
        entity->mToDraw = false;
            
    }

    void RenderEntities::DrawDebug(Entity* entity)
    {
        rlw::DrawRectangleLinesEx(entity->m_finalTransformRect, 1, Color::Cyan);


        ::DrawCircle(entity->m_finalTransformRect.x, entity->m_finalTransformRect.y, 5, ::Color{255,0,0,50});
        if (Math::CheckCollisionPointRec(RayWindow::GetVirtualMouse(), entity->m_finalTransformRect))
        {
            rlw::DrawRectangleLinesEx(entity->m_finalTransformRect, 1, Color::Red);
            // ::DrawRectangle(entity->m_finalTransformRect.x, 
            //                 entity->m_finalTransformRect.y, 
            //                 entity->m_finalTransformRect.width, 
            //                 entity->m_finalTransformRect.height,
            //                 ::Color{255,0,0,50});
            if (Mouse::IsPressed(Mouse::RIGHT))
            {


                LOG(LOG_CLASSIC_DEBUG, std::format("\n-> positions nameID: {}\n-> x {}\n-> y {}\n-> width {}\n-> height {}", 
                    entity->mNameId, 
                    entity->m_transform.position.x,
                    entity->m_transform.position.y,
                    entity->m_transform.position.width,
                    entity->m_transform.position.height
                ).c_str());
                LOG(LOG_CLASSIC_DEBUG, std::format("\n-> world positions nameID: {}\n-> x {}\n-> y {}\n-> width {}\n-> height {}", 
                    entity->mNameId, 
                    entity->m_worldTransform.position.x,
                    entity->m_worldTransform.position.y,
                    entity->m_worldTransform.position.width,
                    entity->m_worldTransform.position.height
                ).c_str());
                LOG(LOG_CLASSIC_DEBUG, std::format("\n-> finaltransform nameID: {}\n-> x {}\n-> y {}\n-> width {}\n-> height {}", 
                    entity->mNameId, 
                    entity->m_finalTransformRect.x,
                    entity->m_finalTransformRect.y,
                    entity->m_finalTransformRect.width,
                    entity->m_finalTransformRect.height
                ).c_str());
                std::cin.get();
            }

        }

    }


} // namespace ClassicLauncher
