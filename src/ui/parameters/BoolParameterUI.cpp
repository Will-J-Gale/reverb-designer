#include <ui/parameters/BoolParameterUI.h>
#include <utils/Constants.h>

BoolParameterUI::BoolParameterUI()
{
    name.setTopLeftPosition(BOOL_PARAMETER_START_X, 0);
    toggle.setBounds(PARAMETER_X_OFFSET, 0, BOOL_PARAMETER_WIDTH, BOOL_PARAMETER_HEIGHT);
    setInterceptsMouseClicks(false, true);
    addAndMakeVisible(toggle);
}

Component* BoolParameterUI::getComponent()
{
    return &toggle;
}

float BoolParameterUI::getVisibleWidth()
{
    return toggle.getWidth() + getNameTextWidth();
}

void BoolParameterUI::addListener(ToggleButton::Listener* listener)
{
    toggle.addListener(listener);
}

void BoolParameterUI::setToggleState(bool state)
{
    toggle.setToggleState(state, dontSendNotification);
}

