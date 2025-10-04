#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include "Entity/Entity.h"
#include "Animations/Animatable.h"
#include "Utils/TimerManager.h"

namespace ClassicLauncher
{

    class GuiFrame : public Entity,  public Animatable
    {
    public:

        GuiFrame();
        ~GuiFrame() = default;
        void SetFrame(bool bForce = false);
        void Click();
        virtual void AnimationFinished(const std::string& name) override;
        virtual EntityType GetType() const override { return EntityType::GuiFrameClass; }
        virtual void Update() override;
        void SetLimitArea(RectFloat area);

    private:

        RectFloat m_limitAreaMove{};
        TimerHandling m_timer;
        bool m_isMove{false};
    };

} // namespace ClassicLauncher

#endif