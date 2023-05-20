#include <ui/parameters/BoolParameterUI.h>
#include <utils/Constants.h>

BoolParameterUI::BoolParameterUI()
{
    name.setTopLeftPosition(30, 0);
    toggle.setBounds(PARAMETER_X_OFFSET, 0, 70, 15);
    setInterceptsMouseClicks(false, true);
    addAndMakeVisible(toggle);
}

Component* BoolParameterUI::getComponent()
{
    return &toggle;
}

void BoolParameterUI::addListener(ToggleButton::Listener* listener)
{
    toggle.addListener(listener);
}

void BoolParameterUI::setToggleState(bool state)
{
    toggle.setToggleState(state, dontSendNotification);
}

