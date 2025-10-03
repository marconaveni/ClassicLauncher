#ifndef GUI_SIZE_BOX
#define GUI_SIZE_BOX

#include "Guis/Components/GuiCanvas.h"

namespace ClassicLauncher
{

    class GuiSizeBox : public GuiCanvas
    {
    public:

        GuiSizeBox();
        ~GuiSizeBox() = default;
        virtual EntityType GetType() const override { return EntityType::GuiSizeBoxClass; }
        virtual void AddChild(Entity* child) override;
        virtual void RemoveChild(Entity* childEntity) override;
        virtual void Update() override;

    private:

        Entity* m_entity;
    };

} // namespace ClassicLauncher

#endif