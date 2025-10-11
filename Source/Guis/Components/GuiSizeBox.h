#ifndef GUI_SIZE_BOX
#define GUI_SIZE_BOX

#include "Guis/Components/GuiCanvas.h"

namespace ClassicLauncher
{
    enum class Margin
    {
        NONE,
        AUTO,
        TOP_ONLY,
        LEFT_ONLY
    };

    class GuiSizeBox : public GuiCanvas
    {
    public:

        GuiSizeBox();
        ~GuiSizeBox() = default;
        virtual EntityType GetType() const override { return EntityType::GuiSizeBoxClass; }
        virtual void AddChild(Entity* child) override;
        virtual void RemoveChild(Entity* childEntity) override;
        virtual void Update() override;
        void SetMargin(Margin margin);
        void SetAspectRatio(bool aspectRatio);

    private:

        void UpdateMargin();
        Margin m_margin{Margin::AUTO};
        Entity* m_entity{nullptr};
        bool m_aspectRatio{true};
    };

} // namespace ClassicLauncher

#endif