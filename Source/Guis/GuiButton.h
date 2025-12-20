#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H


#include "Animations/Animatable.h"
#include "Components/FocusComponent.h"
#include "Guis/Components/GuiCanvas.h"

namespace ClassicLauncher
{

    class GuiCanvas;
    class GuiBase;

    class GuiButton : public GuiCanvas, public FocusComponent, public Animatable
    {
    public:

        GuiButton(FocusManager* focusManagerRef);
        ~GuiButton() = default;
        void Init(const Vector2f sourceIcon);
        virtual EntityType GetType() const override { return EntityType::GuiButtonClass; }

        void Update() override;
        virtual void OnFocus() override;
        virtual void OnLostFocus(FocusCategory previousFocusCategory) override;
        virtual const Transform& OwnerWorldTransform() const override { return GetWorldTransform(); };
        void SizeButton(Sizef size);

    private:

        GuiBase* m_background{nullptr};
        GuiBase* m_icon{nullptr};
    };

} // namespace ClassicLauncher


#endif // GUI_BUTTON_H