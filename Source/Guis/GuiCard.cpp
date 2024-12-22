#include "GuiCard.h"
#include "Application.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(int x, int y)
    {

        mProperties.x = x;
        mProperties.y = y;

        CreateCard(mCardMain, 528, 15, 255);
        CreateCard(mCardFavorite, 783, 15, 0);
        CreateCard(mCardSelected, 273, 15, 0);
        CreateCard(mCover, 0, 0, 255);

        SetCover();
    }

    void GuiCard::CreateCard(std::shared_ptr<GuiComponent>& card, int sourceX, int sourceY, unsigned char alpha)
    {
        Application* app = &Application::Get();

        card = app->GetEntityManager()->CreateEntity<GuiComponent>("card");
        card->mProperties.width = 246;
        card->mProperties.height = 270;
        card->mProperties.sourceX = sourceX;
        card->mProperties.sourceY = sourceY;
        card->mProperties.color.SetOpacity(alpha);
        card->mTextureName = "sprite";

        AddChild(card.get());
    }

    void GuiCard::Update()
    {
        GuiComponent::Update();

        Application* pApplication = &Application::Get();
        Texture2D* textureReference = pApplication->GetSpriteManager()->GetTexture(mCover->mTextureName);

        if (textureReference != nullptr && mCover->mTextureName != "sprite")
        {
            mCover->mProperties.x = (240.0f - static_cast<float>(textureReference->width)) / 2.0f;
            mCover->mProperties.y = (216.0f - static_cast<float>(textureReference->height)) / 2.0f;
        }
    }

    void GuiCard::SetFocus()
    {
        bFocus = true;
        mCardSelected->mProperties.color.SetOpacity(255);
        mCardSelected->mProperties.color.SetOpacity(255);
    }

    void GuiCard::RemoveFocus()
    {
        bFocus = false;
        mCardSelected->mProperties.color.SetOpacity(0);
    }

    void GuiCard::SetCover(std::string name)
    {
        if (name.empty())
        {
            mCover->mProperties.width = 144;
            mCover->mProperties.height = 114;
            mCover->mProperties.sourceX = 1116;
            mCover->mProperties.sourceY = 1131;
            mCover->mProperties.x = 50;
            mCover->mProperties.y = 50;
            mCover->mTextureName = "sprite";

        }
        else
        {
            mCover->mProperties.width = 0;
            mCover->mProperties.height = 0;
            mCover->mProperties.sourceX = 0;
            mCover->mProperties.sourceY = 0;
            mCover->mProperties.x = 0;
            mCover->mProperties.y = 0;
            mCover->mTextureName = name;
        }

    }

    bool GuiCard::IsFocus()
    {
        return bFocus;
    }

}  // namespace ClassicLauncher
