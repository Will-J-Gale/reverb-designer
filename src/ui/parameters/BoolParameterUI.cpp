#include <ui/parameters/BoolParameterUI.h>
#include <utils/Constants.h>

BoolParameterUI::BoolParameterUI()
{
    _name.setTopLeftPosition(BOOL_PARAMETER_START_X, 0);
    _toggle.setBounds(PARAMETER_X_OFFSET, 0, BOOL_PARAMETER_WIDTH, BOOL_PARAMETER_HEIGHT);
    setInterceptsMouseClicks(false, true);
    addAndMakeVisible(_toggle);
}

Component* BoolParameterUI::getComponent()
{
    return &_toggle;
}

float BoolParameterUI::getVisibleWidth()
{
    return _toggle.getWidth() + getNameTextWidth();
}

void BoolParameterUI::addListener(ToggleButton::Listener* listener)
{
    _toggle.addListener(listener);
}

void BoolParameterUI::setToggleState(bool state)
{
    _toggle.setToggleState(state, dontSendNotification);
}

