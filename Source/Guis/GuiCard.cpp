#include "GuiCard.h"
#include "Application.h"


namespace ClassicLauncher
{
    GuiCard::GuiCard(int x, int y)
    {
        this->x = x;
        this->y = y;
        Application* app = &Application::Get();
        cardMain = app->GetEntityManager()->CreateEntity<GuiComponent>();
        cardSelected = app->GetEntityManager()->CreateEntity<GuiComponent>();
        cardFavorite = app->GetEntityManager()->CreateEntity<GuiComponent>();
        cover = app->GetEntityManager()->CreateEntity<GuiComponent>();
        
        cardMain->textureName = "sprite";
        cardSelected->textureName = "sprite";
        cardFavorite->textureName = "sprite";
        cover->textureName = "sprite";

        cardMain->width = 246;
        cardMain->height = 270;
        cardMain->sourceX = 528;
        cardMain->sourceY = 15;


        cardSelected->width = 246;
        cardSelected->height = 270;
        cardSelected->sourceX = 273;
        cardSelected->sourceY = 15;

        cardFavorite->width = 246;
        cardFavorite->height = 270;
        cardFavorite->sourceX = 783;
        cardFavorite->sourceY = 15;

        cover->width = 144;
        cover->height = 114;
        cover->sourceX = 1116;
        cover->sourceY = 1131;
        cover->x = 50;
        cover->y = 50;
        


        AddChild(cardMain.get());
        AddChild(cardSelected.get());
        AddChild(cardFavorite.get());
        AddChild(cover.get());
        
    }

    void GuiCard::Update()
    {
        GuiComponent::Update();
        //x++;
        //TraceLog(LOG_WARNING, "relative x %d" , relativeX);
    }

    void GuiCard::SetFocus()
    {
        bFocus = true;
    }

    bool GuiCard::IsFocus()
    {
        return bFocus;
    }

} // namespace ClassicLauncher

