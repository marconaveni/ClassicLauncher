#ifndef GUI_MINI_COVER_H
#define GUI_MINI_COVER_H

#include <vector>
#include <memory>
#include <string>

#include "GuiComponent.h"
#include "Application.h"

namespace ClassicLauncher
{

    class GuiComponent;

    class GuiMiniCover : public GuiComponent
    {
    private:

        Application* app;
        std::vector<std::shared_ptr<GuiComponent>> guiCovers;

    public:

        GuiMiniCover();
        virtual ~GuiMiniCover() override = default;
        EntityType GetType() const override { return EntityType::GuiMiniCoverClass; }
        void Init();
        void Update() override;
        void End() override;
        void SetPosition(int numCovers);
        void SetCovers();
        void SetCover(std::string name, GuiComponent* miniCover);
        void ClearCovers();

    };




}

#endif // GUI_MINI_COVER_H