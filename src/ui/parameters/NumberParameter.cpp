#include <ui/parameters/NumberParameter.h>
#include <utils/Constants.h>

NumberParameter::NumberParameter()
{
    value.setBounds(PARAMETER_X_OFFSET, 0, NUMBER_WIDTH, PARAMETER_HEIGHT);
    name.setBounds(PARAMETER_X_OFFSET + NUMBER_WIDTH, 0, PARAMETER_TEXT_WIDTH, PARAMETER_HEIGHT);
    name.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(value);
}

void NumberParameter::setValue(double number)
{
    value.setText(String::formatted("%.2f", number), dontSendNotification);
}

void NumberParameter::setWidth(int width)
{
    value.setSize(width, value.getHeight());
    name.setTopLeftPosition(width + NUMBER_TEXT_GAP, name.getY());
}

void NumberParameter::addListener(TextEditor::Listener* listener)
{
    value.addListener(listener);
}

Component* NumberParameter::getComponent()
{
    return &value;
}
