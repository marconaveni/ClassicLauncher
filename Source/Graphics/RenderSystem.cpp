#include "RenderSystem.h"
#include "Application.h"
#include "Core.h" 
#include "rl_wrap.h"

namespace ClassicLauncher
{

#ifdef _DEBUG
    static bool bEnable = false;
#endif

    RenderSystem::RenderSystem(SpriteManager* spriteManagerReference)
        : mSpriteManagerReference(spriteManagerReference)
    {
    }

    void RenderSystem::DrawEntities(const std::vector<std::unique_ptr<Entity>>& entities)
    {
#ifdef _DEBUG

        if (rlw::IsKeyReleased(rlw::KEY_FIVE))
        {
            bEnable = !bEnable;
        }
#endif
        for (auto& entity : entities)
        {
            DrawEntity(entity.get());
        }
    }

    void RenderSystem::DrawEntity(Entity* entity)
    {
        const Texture2D* texture = mSpriteManagerReference->GetTexture(entity->mTextureName);

        //if (texture && entity->mToDraw && entity->mTextureName != "transparent")  // todo verify render
        if (texture && entity->mToDraw)
        {
            entity->mTransform.SetTransforms(Themes::GetScaleTexture());

            if (entity->mScissorMode)
            {
                RectangleClassic scissorArea = entity->mScissorArea;
                scissorArea.width = scissorArea.width * entity->mTransform.GetRootScale().x * Themes::GetScaleTexture();
                scissorArea.height = scissorArea.height * entity->mTransform.GetRootScale().y * Themes::GetScaleTexture();
                BeginScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
            }

            rlw::DrawTexturePro(
                *texture, entity->mTransform.GetSource(), entity->mTransform.GetTransform(), Vector2f{ 0, 0 }, entity->mTransform.rotation, entity->mTransform.color);
            entity->Draw();
            DrawDebug(entity);

            if (entity->mScissorMode)
            {
                EndScissorMode();
                entity->DisableScissorMode();
            }
            entity->mToDraw = false;
        }
    }

    void RenderSystem::DrawDebug(Entity* entity)
    {
#ifdef _DEBUG

        const RectangleClassic& RectangleDrawArea = entity->mTransform.GetTransform();  //{ x, y, scale.x, scale.y };
        const Vector2f vec = Application::Get().GetRender()->GetMousePositionRender();
        rlw::Rectangle point = {RectangleDrawArea.x, RectangleDrawArea.y, RectangleDrawArea.width, RectangleDrawArea.height};
        if (CheckCollisionPointRec(vec, point) && bEnable)
        {
            rlw::DrawRectangleLinesEx(RectangleDrawArea, 2, ColorClassic::Red());
            if (IsMouseButtonPressed( rlw::MOUSE_BUTTON_LEFT))
            {
                PRINT(TEXT("nameID: %s", entity->mNameId.c_str()), 5.0f);
            }
        }
        else if (bEnable)
        {
            rlw::DrawRectangleLinesEx(RectangleDrawArea, 1, ColorClassic::Cyan());
        }
        if (entity->mScissorMode && bEnable)
        {
            const ColorClassic tint = ColorClassic(255, 0, 0, 55);
            RectangleClassic scissorArea = entity->mScissorArea;
            scissorArea.width = scissorArea.width * entity->mTransform.GetRootScale().x * Themes::GetScaleTexture();
            scissorArea.height = scissorArea.height * entity->mTransform.GetRootScale().y * Themes::GetScaleTexture();
            DrawRectangle(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height, tint);
        }
#endif  // _DEBUG
    }

    bool RenderSystem::CheckRender(const RectangleClassic& rec)
    {
        return rlw::CheckCollisionRecs(RectangleClassic(0, 0, 1280, 720), rec);
    }

}  // namespace ClassicLauncher
