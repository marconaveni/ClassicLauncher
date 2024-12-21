#include "GuiCard.h"
#include "Application.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(int x, int y)
    {

        this->mProperties.x = x;
        this->mProperties.y = y;

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
        card->mProperties.alpha = alpha;
        card->textureName = "sprite";

        AddChild(card.get());
    }

    void GuiCard::Update()
    {
        GuiComponent::Update();

        Application* pApplication = &Application::Get();
        Texture2D* textureReference = pApplication->GetSpriteManager()->GetTexture(mCover->textureName);

        if (textureReference != nullptr && mCover->textureName != "sprite")
        {
            mCover->mProperties.x = (240.0f - static_cast<float>(textureReference->width)) / 2.0f;
            mCover->mProperties.y = (216.0f - static_cast<float>(textureReference->height)) / 2.0f;
        }
    }

    void GuiCard::SetFocus()
    {
        bFocus = true;
        mCardSelected->mProperties.alpha = 255;
        mCardSelected->mProperties.alpha = 255;
    }

    void GuiCard::RemoveFocus()
    {
        bFocus = false;
        mCardSelected->mProperties.alpha = 0;
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
            mCover->textureName = "sprite";

        }
        else
        {
            mCover->mProperties.width = 0;
            mCover->mProperties.height = 0;
            mCover->mProperties.sourceX = 0;
            mCover->mProperties.sourceY = 0;
            mCover->mProperties.x = 0;
            mCover->mProperties.y = 0;
            mCover->textureName = name;
        }

    }

    bool GuiCard::IsFocus()
    {
        return bFocus;
    }

}  // namespace ClassicLauncher
