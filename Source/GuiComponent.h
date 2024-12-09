#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H

#include "Entity/Entity.h"
#include <memory>
#include <vector>

namespace ClassicLauncher
{
    class GuiComponent : public Entity 
    {
    private:
	    std::vector<GuiComponent*> entityChildrens;
    public:
        GuiComponent() = default;
        virtual ~GuiComponent() = default;
        virtual EntityType GetType() const override { return EntityType::GuiComponent; }
        virtual void Update();
        virtual void AddChild(GuiComponent* entityChildren);
        virtual void RemoveChild();
        virtual void SelfDelete() override;

    };

}

#endif // GUICOMPONENT_H