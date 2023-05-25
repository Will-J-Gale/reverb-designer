#include <ui/parameters/NumberParameterUI.h>
#include <utils/Constants.h>

NumberParameterUI::NumberParameterUI()
{
    _value.setBounds(PARAMETER_X_OFFSET, 0, NUMBER_WIDTH, PARAMETER_HEIGHT);
    _name.setBounds(PARAMETER_X_OFFSET + NUMBER_WIDTH, 0, PARAMETER_TEXT_WIDTH, PARAMETER_HEIGHT);
    _name.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(_value);
}

void NumberParameterUI::setValue(double number)
{
    _value.setText(String::formatted("%.2f", number), dontSendNotification);
}

void NumberParameterUI::setWidth(int width)
{
    _value.setSize(width, _value.getHeight());
    _name.setTopLeftPosition(width + NUMBER_TEXT_GAP, _name.getY());
}

void NumberParameterUI::addListener(TextEditor::Listener* listener)
{
    _value.addListener(listener);
}

Component* NumberParameterUI::getComponent()
{
    return &_value;
}

float NumberParameterUI::getVisibleWidth()
{
    return _value.getWidth() + getNameTextWidth();
}
