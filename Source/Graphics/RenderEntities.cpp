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
#include "Utils/ConfigurationManager.h"
#include <iostream>

namespace ClassicLauncher
{

#ifdef _DEBUG
    static bool s_enableDebug = false;
    static bool s_disableCache = false;
    static bool s_isThemeEnable = false;
    static Texture s_texture;
#endif

    RenderEntities::RenderEntities(SpriteManager* spriteManagerReference, ConfigurationManager* configManager)
        : m_spriteManagerReference(spriteManagerReference)
        , m_configManagerReference(configManager)
    {
#ifdef _DEBUG
        s_isThemeEnable = m_configManagerReference->GetThemeReferenceOverlay();
#endif
    }

    void RenderEntities::DrawEntities(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        m_renderScale = ThemesManager::GetScaleRenderer();

        for (const auto& entity : entities)
        {
            DrawEntity(entity.get());
        }

#ifdef _DEBUG

        if (Keyboard::IsReleased(Keyboard::Key::FIVE))
        {
            s_enableDebug = !s_enableDebug;
        }
        if (Keyboard::IsReleased(Keyboard::Key::SIX))
        {
            s_disableCache = !s_disableCache;
        }
        DrawThemeReference();
#endif
    }

    void RenderEntities::DrawEntity(Entity* entity)
    {

        Texture* texture = nullptr;
        
        if (entity->m_isCanDraw && entity->m_textureName != "transparent")
        {
            texture = m_spriteManagerReference->GetTexture(entity->m_textureName);
        }
    
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

            entity->m_isTransformDirty = false;
                
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

        if (texture)
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
        if (s_enableDebug)
        {
            DrawDebug(entity);
        }
#endif
        entity->m_isCanDraw = false;
            
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
            if (Mouse::IsReleased(Mouse::LEFT))
            {
                DrawStatistics(entity);
            }

        }

    }

    void RenderEntities::DrawStatistics(Entity* entity)
    {
        const std::string text = R"(
nameID: {}
parent NameID: {}
texture Name: {}
m_childEntities Size: {}
ZOrder id: {}
ZOrder insertionIndex: {}
m_transform position x: {}
m_transform position y: {}
m_transform position width: {}
m_transform position height: {}
m_worldTransform position x: {}
m_worldTransform position y: {}
m_worldTransform position width: {}
m_worldTransform position height: {}
m_finalRender position x: {}
m_finalRender position y: {}
m_finalRender position width: {}
m_finalRender position height: {}
)";

        std::string parent = entity->m_parent ? entity->m_parent->m_nameId : "null";
        int size = static_cast<int>(entity->m_childEntities.size());
        std::string textFinal =  std::vformat(text, std::make_format_args(
            entity->m_nameId, 
            parent, 
            entity->m_textureName, 
            size, 
            entity->m_zOrder.id,
            entity->m_zOrder.insertionIndex,
            entity->m_transform.position.x,
            entity->m_transform.position.y,
            entity->m_transform.position.width,
            entity->m_transform.position.height,
            entity->m_worldTransform.position.x,
            entity->m_worldTransform.position.y,
            entity->m_worldTransform.position.width,
            entity->m_worldTransform.position.height, 
            entity->m_finalRender.transform.x,
            entity->m_finalRender.transform.y,
            entity->m_finalRender.transform.width,
            entity->m_finalRender.transform.height
        ));
        
        LOG(LOG_CLASSIC_DEBUG, "%s", textFinal.c_str());
    }

    void RenderEntities::DrawThemeReference()
    {
#ifdef _DEBUG        
        if (!s_isThemeEnable)
        {
            return;
        }
        
        if (!s_texture.IsValid())
        {
            s_texture.LoadFromFile(m_configManagerReference->GetThemeReferenceImage());
            s_isThemeEnable = s_texture.IsValid();
        }
        else
        {
            rlw::DrawTexture(s_texture, 0, 0, Color{255, 255, 255, 120});
        }
#endif
    }


} // namespace ClassicLauncher
