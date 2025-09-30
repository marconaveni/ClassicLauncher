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
       // const Texture* texture = &textureTeste;


        if (entity->m_isTransformDirty)
        {
            //LOG(LOG_CLASSIC_DEBUG, "nameID: is dirty %s", entity->mNameId.c_str());
            
            entity->m_finalRender.source = RectFloat{entity->m_transform.source.x * m_renderScale,
                entity->m_transform.source.y * m_renderScale,
                entity->m_transform.source.width * m_renderScale,
                entity->m_transform.source.height * m_renderScale};
                
                
            entity->m_finalRender.transform = RectFloat{
                (entity->m_worldTransform.position.x + (entity->m_worldTransform.offset.x * entity->m_worldTransform.scale.x)) * m_renderScale,
                (entity->m_worldTransform.position.y + (entity->m_worldTransform.offset.y * entity->m_worldTransform.scale.y)) * m_renderScale,
                entity->m_transform.position.width * entity->m_worldTransform.scale.x * m_renderScale, // width base * scale final
                entity->m_transform.position.height * entity->m_worldTransform.scale.y * m_renderScale // height base * scale final
            };
                
                
            entity->m_finalRender.origin = Vector2f {
                entity->m_transform.origin.x * entity->m_worldTransform.scale.x * m_renderScale,
                entity->m_transform.origin.y * entity->m_worldTransform.scale.y * m_renderScale
            };

            //entity->m_isTransformDirty = false;
                
        }
        
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
        if (texture && entity->mToDraw && entity->mTextureName != "transparent")
        {


            rlw::DrawTexturePro(*texture,
                                entity->m_finalRender.source,         /* RectFloat{0, 562, 21, 720}, posição spritesheet */
                                entity->m_finalRender.transform, /* RectFloat{0, 0, 1280, 720} posx posy tam_rect  larg_rect */
                                entity->m_finalRender.origin,
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
        rlw::DrawRectangleLinesEx(entity->m_finalRender.transform, 1, Color::Cyan);


        ::DrawCircle(entity->m_finalRender.transform.x, entity->m_finalRender.transform.y, 5, ::Color{255,0,0,50});
        if (Math::CheckCollisionPointRec(RayWindow::GetVirtualMouse(), entity->m_finalRender.transform))
        {
            rlw::DrawRectangleLinesEx(entity->m_finalRender.transform, 1, Color::Red);
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
                    entity->m_finalRender.transform.x,
                    entity->m_finalRender.transform.y,
                    entity->m_finalRender.transform.width,
                    entity->m_finalRender.transform.height
                ).c_str());
                std::cin.get();
            }

        }

    }


} // namespace ClassicLauncher
