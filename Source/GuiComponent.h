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
	
        std::vector<GuiComponent*> childEntities;

    protected:
     
        GuiComponent* parent = nullptr;
        
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

}

#endif // GUI_COMPONENT_H