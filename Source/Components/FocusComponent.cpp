#include "FocusComponent.h"
#include "FocusManager.h"
#include "Application.h"
#include "Entity/Entity.h"

namespace ClassicLauncher
{
    FocusComponent::FocusComponent(Application* application, Entity* entity)
    : mApplication(application), mEntity(entity)
    {  
        mApplication->GetFocusManager()->AddFocus(this);
    }
    
    FocusComponent::~FocusComponent()
    {
        mApplication->GetFocusManager()->RemoveFocus(this);
    }

    void FocusComponent::SetFocus()
    {
        mApplication->GetFocusManager()->UpdateFocus(this);
        mIsFocus = true;
    }


}  // namespace ClassicLauncher
