#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "Entity/Entity.h"
#include <memory>
#include <vector>

namespace ClassicLauncher
{
    class GuiComponent : public Entity 
    {
    private:
	    std::vector<GuiComponent*> entityChildrens;
        GuiComponent* parent;
    public:
        GuiComponent() = default;
        virtual ~GuiComponent() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiComponentClass; }
        void Update() override;
        virtual void AddChild(GuiComponent* entityChildren);
        virtual void RemoveChild();
        virtual void SelfDelete() override;

    };

}

#endif // GUI_COMPONENT_H