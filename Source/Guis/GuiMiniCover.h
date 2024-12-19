#ifndef GUI_MINI_COVER_H
#define GUI_MINI_COVER_H

#include <memory>
#include <string>
#include <vector>

#include "Application.h"
#include "GuiComponent.h"

namespace ClassicLauncher
{

    class GuiComponent;

    class GuiMiniCover : public GuiComponent
    {
    private:

        Application* mApplication;
        std::vector<std::shared_ptr<GuiComponent>> mGuiCovers;

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

}  // namespace ClassicLauncher

#endif  // GUI_MINI_COVER_H