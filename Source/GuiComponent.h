#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <memory>
#include <vector>
#include "Entity/Entity.h"

namespace ClassicLauncher
{
    class GuiComponent : public Entity
    {
    private:

        std::vector<GuiComponent*> mChildEntities;

    protected:

        GuiComponent* mParent = nullptr;

    public:

        GuiComponent() = default;
        virtual ~GuiComponent() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiComponentClass; }
        virtual void Update() override;
        virtual void UpdatePosition() override;
        virtual void AddChild(GuiComponent* entityChildren);
        virtual void RemoveChild();
        virtual std::vector<GuiComponent*> GetChilds();
        virtual void SelfDelete() override;
        GuiComponent* GetRootEntity();
    };

}  // namespace ClassicLauncher

#endif  // GUI_COMPONENT_H