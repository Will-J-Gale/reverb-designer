#include <ui/parameters/BaseParameterUI.h>
#include <utils/Constants.h>

BaseParameterUI::BaseParameterUI()
{
    setBounds(0, 0, PARAMETER_WIDTH, PARAMETER_HEIGHT);

    name.setJustificationType(Justification::centredLeft);
    name.setBounds(0, 0, PARAMETER_NAME_WIDTH, PARAMETER_NAME_HEIGHT);
    name.setInterceptsMouseClicks(false, false);
    name.setColour(Label::textColourId, Colour::fromString(TEXT_COLOUR));
    name.setFont(Font(FONT_SIZE, Font::bold));
    addAndMakeVisible(name);
}

void BaseParameterUI::setParameterName(String newName)
{
    name.setText(newName, NotificationType::dontSendNotification);
    // setBounds(0, 0, getVisibleWidth(), PARAMETER_HEIGHT);
}

std::string BaseParameterUI::getId()
{
    return id.toString().toStdString();
}

float BaseParameterUI::getNameTextWidth()
{
    auto width = name.getFont().getStringWidthFloat(name.getText());
    return width < PARAMETER_MAX_SIZE ? width : PARAMETER_MAX_SIZE;
}

