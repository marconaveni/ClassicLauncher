#include "RenderSystem.h"
#include "Application.h"

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

        if (IsKeyReleased(KEY_FIVE))
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
        if (texture && entity->mToDraw)
        {
            //entity->mTransform.rotation++;
            entity->mTransform.SetTransforms(Themes::GetScaleTexture());

            if (entity->mScissorMode)
            {
                Rectangle scissorArea = entity->mScissorArea;
                scissorArea.width = scissorArea.width * entity->mTransform.rootScaleX * Themes::GetScaleTexture();
                scissorArea.height = scissorArea.height * entity->mTransform.rootScaleY * Themes::GetScaleTexture();
                BeginScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
            }

            ::DrawTexturePro(*texture, entity->mTransform.GetSource(), entity->mTransform.GetTransform() , Vector2{ 0, 0 }, entity->mTransform.rotation, entity->mTransform.color);
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

        const Rectangle& RectangleDrawArea = entity->mTransform.GetTransform();  //{ x, y, scale.x, scale.y };
        const Vector2 vec = Application::Get().GetRender()->GetMousePositionRender();
        if (CheckCollisionPointRec(vec, RectangleDrawArea) && bEnable)
        {
            ::DrawRectangleLinesEx(RectangleDrawArea, 2, Color::Red());
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                PRINT(TEXT("nameID: %s", entity->mNameId.c_str()), 5.0f);
            }
        }
        else if (bEnable)
        {
            ::DrawRectangleLinesEx(RectangleDrawArea, 1, Color::Cyan());
        }
        if (entity->mScissorMode && bEnable)
        {
            const Color tint = Color(255, 0, 0, 55);
            Rectangle scissorArea = entity->mScissorArea;
            scissorArea.width = scissorArea.width * entity->mTransform.rootScaleX * Themes::GetScaleTexture();
            scissorArea.height = scissorArea.height * entity->mTransform.rootScaleY * Themes::GetScaleTexture();
            DrawRectangle(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height, tint);
        }
#endif  // _DEBUG
    }

}  // namespace ClassicLauncher
