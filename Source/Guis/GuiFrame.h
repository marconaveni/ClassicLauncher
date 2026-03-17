#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include "Animations/Animatable.h"
#include "Entity/Entity.h"
#include "Utils/TimerManager.h"


namespace ClassicLauncher
{
    class GuiBase;
    class Window;

    class GuiFrame : public Entity, public Animatable
    {
    public:

        explicit GuiFrame(const EntityContext& entityContext, Window* window);
        ~GuiFrame() = default;
        void SetFrame(bool force = false);
        void Click();
        virtual void AnimationFinished(const std::string& name) override;
        virtual EntityType GetType() const override { return EntityType::GuiFrameClass; }
        virtual void Update() override;
        void SetLimitArea(RectFloat area);
        [[nodiscard]] bool IsFrameMove();

    private:

        void UpdateFramePosition();

        GuiBase* m_frameMenu{nullptr};
        RectFloat m_limitAreaMove{};
        TimerHandling m_timer{};
        bool m_isMove{false};
    };

} // namespace ClassicLauncher

#endif