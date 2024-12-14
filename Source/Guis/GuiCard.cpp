#include "GuiCard.h"
#include "Application.h"


namespace ClassicLauncher
{


    GuiCard::GuiCard(int x, int y)
    {
        this->x = x;
        this->y = y;

        CreateCard(cardMain, 528, 15, 255);
        CreateCard(cardFavorite, 783, 15, 0);
        CreateCard(cardSelected, 273, 15, 0);
        CreateCard(cover, 0, 0, 255);
        
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
        Texture2D* textureReference = app->GetSpriteManager()->GetTexture(cover->textureName);

        if (textureReference != nullptr)
        {
            //cover->SetTexture(textureReference, Rectangle{ 0,0,static_cast<float>(textureReference->width),static_cast<float>(textureReference->height) });
            cover->x = (240.0f - static_cast<float>(textureReference->width)) / 2.0f;
            cover->y = (216.0f - static_cast<float>(textureReference->height)) / 2.0f;
        }
        else
        {
            //cover->SetTexture(TextureManager::GetInstance()->GetSprite("sprite"), Rectangle{ 1086,1086,204,204 });
            cover->x = (240.0f - 204.0f) / 2.0f;
            cover->y = (216.0f - 204.0f) / 2.0f;
        }
        //x++;
        //TraceLog(LOG_WARNING, "relative x %d" , relativeX);
    }

    void GuiCard::SetFocus()
    {
        bFocus = true;
        cardSelected->alpha = 255;
    }

    void GuiCard::RemoveFocus()
    {
        bFocus = false;
        cardSelected->alpha = 0;
    }

    void GuiCard::SetCover(std::string name)
    {
        if(name.empty())
        {
            cover->width = 144;
            cover->height = 114;
            cover->sourceX = 1116;
            cover->sourceY = 1131;
            cover->x = 50;
            cover->y = 50;
            cover->textureName = "sprite";
        }
        else
        {
            cover->width = 0;
            cover->height = 0;
            cover->sourceX = 0;
            cover->sourceY = 0;
            cover->x = 0;
            cover->y = 0;
            cover->textureName = name;
        }
    }

    bool GuiCard::IsFocus()
    {
        return bFocus;
    }

} // namespace ClassicLauncher

