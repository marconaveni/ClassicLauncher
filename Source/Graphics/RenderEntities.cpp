#include "RenderEntities.h"
#include <format>
#include "Application.h"
#include "Helper.h"
#include "rl_wrap.h"

namespace ClassicLauncher
{

#ifdef _DEBUG
    static bool bEnable = false;
#endif

    RenderEntities::RenderEntities(SpriteManager* spriteManagerReference)
        : mSpriteManagerReference(spriteManagerReference)
    {
    }

    void RenderEntities::DrawEntities(const std::vector<std::unique_ptr<Entity>>& entities)
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

    void RenderEntities::DrawEntity(Entity* entity)
    {
        const rlw::Texture2D* texture = mSpriteManagerReference->GetTexture(entity->mTextureName);

        // if (texture && entity->mToDraw && entity->mTextureName != "transparent")  // todo verify render
        if (texture && entity->mToDraw)
        {
            entity->mTransform.SetTransforms(Themes::GetScaleTexture());

            if (entity->mScissorMode)
            {
                RectFloat scissorArea = entity->mScissorArea;
                scissorArea.width = scissorArea.width * entity->mTransform.root.scale.x * Themes::GetScaleTexture();
                scissorArea.height = scissorArea.height * entity->mTransform.root.scale.y * Themes::GetScaleTexture();
                rlw::BeginScissorMode(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height);
            }

            rlw::DrawTexturePro(*texture,
                                entity->mTransform.GetSource(),
                                entity->mTransform.GetTransform(),
                                Vector2f{ 0, 0 },
                                entity->mTransform.rotation,
                                entity->mTransform.color);
            entity->Draw();
            DrawDebug(entity);

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
#ifdef _DEBUG

        const RectFloat& RectangleDrawArea = entity->mTransform.GetTransform();  //{ x, y, scale.x, scale.y };
        const Vector2f vec = Application::Get().GetRenderScreen()->GetMousePositionRender();
        Rectangle point = { RectangleDrawArea.x, RectangleDrawArea.y, RectangleDrawArea.width, RectangleDrawArea.height };
        if (rlw::CheckCollisionPointRec(vec, point) && bEnable)
        {
            rlw::DrawRectangleLinesEx(RectangleDrawArea, 2, Color::Red);
            if (IsMouseButtonPressed(rlw::MOUSE_BUTTON_LEFT))
            {
                PRINT(std::format("nameID: {}", entity->mNameId).c_str(), 5.0f);
            }
        }
        else if (bEnable)
        {
            rlw::DrawRectangleLinesEx(RectangleDrawArea, 1, Color::Cyan);
        }
        if (entity->mScissorMode && bEnable)
        {
            const Color tint = Color(255, 0, 0, 55);
            RectFloat scissorArea = entity->mScissorArea;
            scissorArea.width = scissorArea.width * entity->mTransform.root.scale.x * Themes::GetScaleTexture();
            scissorArea.height = scissorArea.height * entity->mTransform.root.scale.y * Themes::GetScaleTexture();
            rlw::DrawRectangle(scissorArea.x, scissorArea.y, scissorArea.width, scissorArea.height, tint);
        }
#endif  // _DEBUG
    }

    bool RenderEntities::CheckRender(const RectFloat& rec)
    {
        return rlw::CheckCollisionRecs(RectFloat(0, 0, 1280, 720), rec);
    }

}  // namespace ClassicLauncher
