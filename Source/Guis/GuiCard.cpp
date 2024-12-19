#include "GuiCard.h"
#include "Application.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(int x, int y)
    {
        this->x = x;
        this->y = y;

        CreateCard(mCardMain, 528, 15, 255);
        CreateCard(mCardFavorite, 783, 15, 100);
        CreateCard(mCardSelected, 273, 15, 100);
        CreateCard(mCover, 0, 0, 255);

        SetCover();
    }

    void GuiCard::CreateCard(std::shared_ptr<GuiComponent>& card, int sourceX, int sourceY, unsigned char alpha)
    {

        Application* app = &Application::Get();

        card = app->GetEntityManager()->CreateEntity<GuiComponent>();
        card->textureName = "sprite";
        card->width = 246;
        card->height = 270;
        card->sourceX = sourceX;
        card->sourceY = sourceY;
        card->alpha = alpha;

        AddChild(card.get());
    }

    void GuiCard::Update()
    {
        GuiComponent::Update();

        Application* app = &Application::Get();
        Texture2D* textureReference = app->GetSpriteManager()->GetTexture(mCover->textureName);

        if (textureReference != nullptr && mCover->textureName != "sprite")
        {
            mCover->x = (240.0f - static_cast<float>(textureReference->width)) / 2.0f;
            mCover->y = (216.0f - static_cast<float>(textureReference->height)) / 2.0f;
        }
    }

    void GuiCard::SetFocus()
    {
        bFocus = true;
        mCardSelected->alpha = 255;
    }

    void GuiCard::RemoveFocus()
    {
        bFocus = false;
        mCardSelected->alpha = 0;
    }

    void GuiCard::SetCover(std::string name)
    {
        if (name.empty())
        {
            mCover->width = 144;
            mCover->height = 114;
            mCover->sourceX = 1116;
            mCover->sourceY = 1131;
            mCover->x = 50;
            mCover->y = 50;
            mCover->textureName = "sprite";
        }
        else
        {
            mCover->width = 0;
            mCover->height = 0;
            mCover->sourceX = 0;
            mCover->sourceY = 0;
            mCover->x = 0;
            mCover->y = 0;
            mCover->textureName = name;
        }
    }

    bool GuiCard::IsFocus() { return bFocus; }

}  // namespace ClassicLauncher
