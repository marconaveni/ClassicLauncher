#include "GuiHorizontalBox.h"

#include "Utils/Math.h"

namespace ClassicLauncher
{

    GuiHorizontalBox::GuiHorizontalBox()
    {
        SetSize(10.0f, 10.0f);
    }

    void GuiHorizontalBox::UpdateWorldTransform()
    {
        Entity::UpdateWorldTransform();
    }

    void GuiHorizontalBox::AttachGui(Entity* guiComponent)
    {
        guiComponent->SetPosition(Vector2f{});
        mGuiElements.push_back(guiComponent);
    }

    void GuiHorizontalBox::ClearAll()
    {
        mGuiElements.clear();
        SetSize(10.0f, 10.0f);
    }

    void GuiHorizontalBox::Update()
    {
        Entity::Update();

        float x = 0;
        float canvasWidth = 0;
        float canvasHeight = 0;
        float spacer = 0;


        for (auto& guiElement : mGuiElements)
        {
            Transform& transform = guiElement->GetTransformRef();
            Transform& worldTransform = guiElement->GetWorldTransformRef();

            spacer += m_spacer;
            transform.position.x = x;
            transform.position.y = 0.0f;
            x += (transform.position.width) + m_spacer;

            canvasWidth += transform.position.width;
            canvasHeight = transform.position.height > canvasHeight ? transform.position.height : canvasHeight;
        }

        spacer -= m_spacer;
        
        const float width = canvasWidth + spacer;
        const float height = canvasHeight;
        SetSize(width, height);
    }

    void GuiHorizontalBox::Draw()
    {
        
    }


} // namespace ClassicLauncher
