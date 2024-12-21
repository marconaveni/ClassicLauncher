#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <map>
#include <memory>
#include <vector>
#include "Entity/Entity.h"
#include "Graphics/SpriteAnimator.h"

namespace ClassicLauncher
{
    class GuiComponent : public Entity
    {

        std::map<std::string, SpriteAnimator> mAnimations;

    public:

        GuiComponent() = default;
        virtual ~GuiComponent() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiComponentClass; }
        virtual void Update() override;

        void AddAnimationFrame(const std::string& name, const float timeAnimation, const std::vector<Rectangle>& spriteIndices);
    };

}  // namespace ClassicLauncher

#endif  // GUI_COMPONENT_H