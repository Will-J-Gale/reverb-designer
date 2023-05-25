#include <ui/parameters/BaseParameterUI.h>
#include <utils/Constants.h>

BaseParameterUI::BaseParameterUI()
{
    setBounds(0, 0, PARAMETER_WIDTH, PARAMETER_HEIGHT);

    _name.setJustificationType(Justification::centredLeft);
    _name.setBounds(0, 0, PARAMETER_NAME_WIDTH, PARAMETER_NAME_HEIGHT);
    _name.setInterceptsMouseClicks(false, false);
    _name.setColour(Label::textColourId, Colour::fromString(TEXT_COLOUR));
    _name.setFont(Font(FONT_SIZE, Font::bold));
    addAndMakeVisible(_name);
}

void BaseParameterUI::setParameterName(String newName)
{
    _name.setText(newName, NotificationType::dontSendNotification);
    // setBounds(0, 0, getVisibleWidth(), PARAMETER_HEIGHT);
}

std::string BaseParameterUI::getId()
{
    return _id.toString().toStdString();
}

float BaseParameterUI::getNameTextWidth()
{
    auto width = _name.getFont().getStringWidthFloat(_name.getText());
    return width < PARAMETER_MAX_SIZE ? width : PARAMETER_MAX_SIZE;
}

