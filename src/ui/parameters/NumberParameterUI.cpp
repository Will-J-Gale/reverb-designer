#include <ui/parameters/NumberParameterUI.h>
#include <utils/Constants.h>

NumberParameterUI::NumberParameterUI()
{
    value.setBounds(PARAMETER_X_OFFSET, 0, NUMBER_WIDTH, PARAMETER_HEIGHT);
    name.setBounds(PARAMETER_X_OFFSET + NUMBER_WIDTH, 0, PARAMETER_TEXT_WIDTH, PARAMETER_HEIGHT);
    name.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(value);
}

void NumberParameterUI::setValue(double number)
{
    value.setText(String::formatted("%.2f", number), dontSendNotification);
}

void NumberParameterUI::setWidth(int width)
{
    value.setSize(width, value.getHeight());
    name.setTopLeftPosition(width + NUMBER_TEXT_GAP, name.getY());
}

void NumberParameterUI::addListener(TextEditor::Listener* listener)
{
    value.addListener(listener);
}

Component* NumberParameterUI::getComponent()
{
    return &value;
}

float NumberParameterUI::getVisibleWidth()
{
    return value.getWidth() + getNameTextWidth();
}
