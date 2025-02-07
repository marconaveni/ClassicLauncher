#ifndef GUI_MINI_COVER_H
#define GUI_MINI_COVER_H

#include <memory>
#include <string>
#include <vector>

#include "Application.h"
#include "GuiComponent.h"
#include "Guis/GuiSizeBox.h"

namespace ClassicLauncher
{

    class GuiComponent;
    class GuiSizeBox;

    class GuiMiniCover : public GuiComponent
    {
    private:

        std::vector<GuiComponent*> mGuiCovers;
        std::vector<GuiSizeBox*> mGuiSizeBoxs;
        GuiComponent* mArrow;
        int mSize;

    public:

        GuiMiniCover();
        virtual ~GuiMiniCover() override = default;
        EntityType GetType() const override { return EntityType::GuiMiniCoverClass; }
        void Init();
        void Update() override;
        void End() override;
        void SetCovers();
        void SetCover(std::string name, GuiComponent* miniCover);
        void ClearCovers();

    private:

        void SetPositionCovers(int numCovers);
    };

}  // namespace ClassicLauncher

#endif  // GUI_MINI_COVER_H