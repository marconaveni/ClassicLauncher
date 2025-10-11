#ifndef GUI_HORIZONTAL_BOX
#define GUI_HORIZONTAL_BOX

#include <vector>

#include "Guis/Components/GuiCanvas.h"

namespace ClassicLauncher
{

    class GuiHorizontalBox : public GuiCanvas
    {
    public:

        GuiHorizontalBox();
        ~GuiHorizontalBox() = default;
        virtual EntityType GetType() const override { return EntityType::GuiHorizontalBoxClass; }
        virtual void UpdateWorldTransform() override;
        void SetSpace(float spacer);
        [[nodiscard]] float GetSpace() const { return m_spacer; };
        void AttachGui(Entity* guiComponent);
        void ClearAll();
        virtual void Update() override;
        virtual void Draw() override;

    private:

        std::vector<Entity*> m_guiElements{};
        float m_spacer{0.0f};
    };

} // namespace ClassicLauncher

#endif